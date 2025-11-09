package tcpdemo

import (
	"bytes"
	"fmt"
	"log"
	"net"
)

func checkErr(err error) {
	if err != nil {
		log.Fatal(err)
	}
}

func readAll(conn net.Conn) ([]byte, error) {
	buf := make([]byte, 512)
	result := bytes.NewBuffer(nil)
	for {
		_, err := conn.Read(buf[:])
		if err != nil {
			if err.Error() == "EOF" {
				break
			}
			return nil, err
		}
		result.Write(buf)
	}
	return result.Bytes(), nil
}

func Solve() {
	conn, err := net.Dial("tcp", "localhost:8000")
	checkErr(err)
	defer conn.Close()

	n, err := conn.Write([]byte("HEAD / HTTP/1.0\r\n\r\n"))
	fmt.Printf("Sent %d bytes\n", n)
	checkErr(err)

	result, err := readAll(conn)
	checkErr(err)
	fmt.Printf("Received %d bytes:\n%s\n", len(result), string(result))
}
