// Clock Server is a concurrent TCP server that periodically writes the time.
package main

import (
	"flag"
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"strconv"
	"time"
)

func handleConn(timeZone string, c net.Conn) {

	defer c.Close()
	for {
		_, err2 := io.WriteString(c, timeZone+" : ")
		if err2 != nil {
			return // e.g., client disconnected
		}
		_, err := io.WriteString(c, time.Now().Format("15:04:05\n"))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {
	// Setting flag's parameter in the terminal
	var nFlag = flag.Int("port", -1, "Please provide a port")
	flag.Parse()
	var localhost string
	localhost = "localhost:" + strconv.Itoa(*nFlag)
	//TZ=US/Eastern : Getting split env terms
	var timeZone = os.Getenv("TZ")

	if len(timeZone) == 0 {
		log.Print("Please provide a time zone example: TZ=<VAR1>/<VAR2> go run clockServer.go -port <port>\n")
		return
	}
	listener, err := net.Listen("tcp", localhost)
	fmt.Println("Clock server has been successfully connected.")
	fmt.Println("Address: ", localhost)
	fmt.Println("Time zone: ", timeZone)
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(timeZone, conn) // handle connections concurrently
	}
}
