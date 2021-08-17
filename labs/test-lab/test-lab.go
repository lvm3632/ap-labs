package main

import (
	"fmt"
	"os"
)

func main() {
	if len(os.Args) > 1 {
		name := ""
		for i := 1; i < len(os.Args); i++ {
			name += os.Args[i] + " "
		}
		fmt.Printf("Welcome %v to the jungle \n", name)
	} else {
		fmt.Printf("The arguments are empty.\nPlease send a name or many.\n")
		return
	}
}
