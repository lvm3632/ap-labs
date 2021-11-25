package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"regexp"
	"strings"
)

func writingTime(socket net.Conn, city string) {
	for {
		event, err := bufio.NewReader(socket).ReadString('\n')
		if err != nil {
			log.Fatal(err)
			os.Exit(1)
		}
		fmt.Printf("\r (%s) : %s", city, event)
	}
}
func main() {
	argv := os.Args
	argc := len(argv)
	//var arr = r.FindAllString("NewYork=localhost:8010 Tokyo=localhost:8040", -1)
	// Getting arr[0]="NewYork=localhost:8010", arr[1]="Tokyo=localhost:8040", and so on...
	if argc < 2 {
		fmt.Println("Please provide more than one parameter")
		fmt.Println("Example: go run clockWall.go <city>=localhost:<port> <city>=localhost:<port> ...")
		fmt.Println("This program can support at least 3 clock servers.")
		return
	}
	localhosts := make([]string, argc)
	cities := make([]string, argc)
	nameServer, _ := regexp.Compile("localhost:[0-9]{1,5}")
	for i := 0; i < argc; i++ {
		localhosts[i] = nameServer.FindString(argv[i])
		cities[i] = strings.Split(argv[i], "=")[0]
	}
	coroutine := make(chan int)
	background := 1
	for i := 0; i < argc; i++ {
		if localhosts[i] == "" {
			continue
		}
		socket, err := net.Dial("tcp", localhosts[i])
		if socket != nil {
			defer socket.Close()
		}
		if err != nil {
			fmt.Println(localhosts[i], " has not been settled yet with clockServer.go")
			log.Fatal(err)
			return
		}
		go writingTime(socket, cities[i])
	}
	background = <-coroutine
	log.Println(background)
}
