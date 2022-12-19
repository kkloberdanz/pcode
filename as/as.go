package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"os"
	"strconv"
	"strings"
)

var opcodes = map[string]uint64{
	"BRK":       0x0000,
	"NOP":       0x0001,
	"HALT":      0x0002,
	"PUSH_I64":  0x0003,
	"POP_I64":   0x0004,
	"PRINT_I64": 0x0005,
	"ADD_I64":   0x0006,
	"SUB_I64":   0x0007,
	"MUL_I64":   0x0008,
	"DIV_I64":   0x0009,
	"JMP":       0x000a,
	"DEC":       0x000b,
	"INC":       0x000c,
	"JZ":        0x000d,
	"JNZ":       0x000e,
	"CALL":      0x000f,
	"RET":       0x0010,
	"JEQ":       0x0011,
	"JLT":       0x0012,
	"JLTE":      0x0013,
	"JGT":       0x0014,
	"JGTE":      0x0015,
}

var labels = map[string]int{}

func opcodeToBin(op string) (uint64, error) {
	code, found := opcodes[op]
	if !found {
		return 0, fmt.Errorf("opcode does not exist: '%s'", op)
	}
	return code, nil
}

func readFile() []string {
	reader := bufio.NewReader(os.Stdin)
	var lines []string
	for {
		line, err := reader.ReadString('\n')
		if err != nil {
			break
		}
		line = strings.TrimSpace(line)
		for _, word := range strings.Split(line, " ") {
			if len(line) > 0 {
				lines = append(lines, word)
			}
		}
	}
	lines = append(lines, "HALT")
	return lines
}

func emit(word uint64) {
	binary.Write(os.Stdout, binary.LittleEndian, word)
}

func findLabels(lines []string) {
	/* new label */
	wordIdx := 0
	for _, line := range lines {
		if line[len(line)-1] == ':' {
			labels[line] = wordIdx
			continue
		}
		wordIdx++
	}
}

func assemble(lines []string) {
	for _, line := range lines {
		/* declare label */
		if line[len(line)-1] == ':' {
			continue
		}

		/* number literal */
		var numCandidate string
		var base int
		if strings.HasPrefix(line, "0x") {
			numCandidate = line[2:]
			base = 16
		} else {
			numCandidate = line
			base = 10
		}
		if num, err := strconv.ParseInt(numCandidate, base, 64); err == nil {
			emit(uint64(num))
			continue
		}

		/* lookup label */
		jmpLocation, found := labels[line+":"]
		if found {
			emit(uint64(jmpLocation))
			continue
		}

		/* opcodes */
		opcode, err := opcodeToBin(line)
		if err != nil {
			panic(err)
		}
		emit(opcode)
	}
}

func main() {
	lines := readFile()
	findLabels(lines)
	assemble(lines)
}
