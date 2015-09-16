.PHONY linux
.PHONY windows
.PHONY n2n
.PHONY manager
.PHONY build

linux: n2n manager
	mkdir manager; cp n2n_build/edge manager; cp n2n-manager-build/n2n-manager manager


n2n:
	mkdir n2n_build; cd n2n_build; cmake -DSNM=1 ../n2n_v2; make

manager:
	mkdir n2n-manager-build; cd n2n-manager-build; qmake ../n2n-manager; make


