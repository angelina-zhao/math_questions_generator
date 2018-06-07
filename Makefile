SRC = main

.PHONY: target_and_run
target_and_run:
	@echo "Start to build..."
	gcc $(SRC).c -o $(SRC).exe
	@echo "Build completed"

.PHONY: clean
clean:
	@echo "Start to clean..."
	rm $(SRC).exe
	@echo "Clean completed"

