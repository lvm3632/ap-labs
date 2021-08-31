package main

import (
	"fmt"
	"os"
	"strconv"
)

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
func calc(operator int, values []int) int {
	total := 0
	if operator == 1 {
		total = 0
		for i := 0; i < len(values); i++ {
			total += values[i]
		}
	} else if operator == 2 {
		total = 0
		for i := 0; i < len(values); i++ {
			if i == 0 {
				total = values[i]
			} else {
				total = total - values[i]
			}
		}
	} else if operator == 3 {
		total = 1
		for i := 0; i < len(values); i++ {
			total *= values[i]
		}
	} else {
		return -1
	}
	return total
}

func main() {
	argVector := os.Args
	argCounter := len(argVector)

	if argCounter <= 3 {
		fmt.Printf("You need to send at least two numbers separeted by a space and an operator\n")
		fmt.Printf("Example\n")
		fmt.Printf("./calculator <operator> <numbers separated by space>\n")
		fmt.Printf("Expressions that are valid: \n")
		fmt.Printf("./calculator add 1 2 3 4 5\n")
		fmt.Printf("./calculator sub 1 2 3 4 5\n")
		fmt.Printf("./calculator mult 1 2 3 4 5\n")
		return

	} else {
		len := argCounter - 2
		arr_numbers := make([]int, len)
		for i := 0; i < len; i++ {
			digit, err := strconv.Atoi(argVector[i+2])
			if err != nil || digit == 0 {
				fmt.Printf("Solo números positivos y/o diferente a 0\n")
			} else {
				arr_numbers[i] = digit
			}
		}

		operator := argVector[1]

		if operator == "add" {
			fmt.Printf("%d\n", calc(1, arr_numbers))
		} else if operator == "sub" {
			fmt.Printf("%d\n", calc(2, arr_numbers))
		} else if operator == "mult" {
			fmt.Printf("%d\n", calc(3, arr_numbers))
		} else {

			fmt.Printf("Expresión inválida, el programa solo soporta: add, sub y mult y/o parámetros pueden ser inválidos \n")
			fmt.Printf("Ejemplo correcto: \n")
			fmt.Printf("./calculator <operator> <numbers separated by space>\n")
			return
		}
	}
}
