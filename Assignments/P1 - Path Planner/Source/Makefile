default:
	@mkdir -p out/
	$(MAKE) -C Framework/TileSystem/
	cp Framework/TileSystem/TileSystem.dll out/
	$(MAKE) -C PathSearch/
	cp PathSearch/PathSearch.dll out/
	$(MAKE) -C Framework/PathPlannerApp/
	cp Framework/PathPlannerApp/PathPlannerApp.exe out/

run: default
	wine out/PathPlannerApp.exe

profile: default
	valgrind --trace-children=yes --vex-iropt-register-updates=allregs-at-mem-access --partial-loads-ok=yes wine out/PathPlannerApp.exe

clean:
	$(MAKE) -C Framework/TileSystem/ clean
	$(MAKE) -C PathSearch/ clean
	$(MAKE) -C Framework/PathPlannerApp/ clean
	rm -f out/TileSystem.dll
	rm -f out/PathSearch.dll
	rm -f out/PathPlannerApp.exe
