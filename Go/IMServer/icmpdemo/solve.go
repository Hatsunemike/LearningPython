package icmpdemo

import (
	"fmt"
	"log"
	"net"
)

func checksum(msg []uint8) uint16 {
	// CRC checksum without 1st byte and last byte
	var sum uint32 = 0
	for i := 0; i < len(msg); i += 2 {
		sum += uint32(msg[i])<<8 + uint32(msg[i+1])
	}
	sum = (sum >> 16) + (sum & 0xffff)
	sum += (sum >> 16)
	return uint16(^sum)
}

func printMsg(msg []uint8, context string) {
	fmt.Print(context)
	for _, v := range msg {
		fmt.Printf("%02x ", v)
	}
	fmt.Printf("\n")
}

func Checkerr(err error) {
	if err != nil {
		log.Fatal(err)
	}
}

func Solve() {
	conn, err := net.Dial("ip4:icmp", "www.baidu.com")
	Checkerr(err)
	var msg [40]uint8

	msg[0] = 8 // Type: Echo Request
	msg[1] = 0 // Code: 0
	msg[2] = 0 // Checksum [0]
	msg[3] = 0 // Checksum [1]
	msg[4] = 0
	msg[5] = 1
	msg[6] = 0
	msg[7] = 1
	var cnt = 97
	for i, _ := range msg[8:] {
		msg[8+i] = uint8(cnt)
		cnt++
		if cnt > (0x77) {
			cnt = 97
		}
	}
	length := 8
	ans := checksum(msg[:length])
	msg[2] = uint8(ans >> 8)
	msg[3] = uint8(ans & 0xff)
	printMsg(msg[:], "msg: ")

	n, err := conn.Write(msg[:length])
	fmt.Printf("write %d bytes\n", n)
	Checkerr(err)
	n, err = conn.Read(msg[:])
	Checkerr(err)
	fmt.Printf("read %d bytes\n", n)
	printMsg(msg[:], "reply msg: ")
}
