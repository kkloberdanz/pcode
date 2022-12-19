.PHONY: all
all: vm as

.PHONY: vm
vm:
	cd vm && $(MAKE)

.PHONY: as
as:
	cd as && go build .

.PHONY: fmt
fmt:
	cd vm && $(MAKE) fmt
	cd as && go fmt as.go

.PHONY: clean
clean:
	cd vm && $(MAKE) clean
	cd as && go clean
