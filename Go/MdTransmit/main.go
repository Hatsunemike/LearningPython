package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
)

type node struct {
	handle func(ch byte) *node
}

func main() {
	fmt.Println("Hello World!")
	file, err := os.Open("./input.md")
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()
	out, _ := os.Create("output.md")
	defer out.Close()
	reader := bufio.NewReader(file)
	writer := bufio.NewWriter(out)
	defer writer.Flush()
	nodes := make([]node, 8)
	var cnt int = 0
	nodes[0].handle = func(ch byte) *node {
		switch ch {
		case '(':
			writer.WriteByte('$')
			return &nodes[1]
		case '$':
			writer.WriteByte(ch)
			b, _ := reader.Peek(1)
			if b[0] == '$' {
				reader.ReadByte()
				writer.WriteByte('$')
				return &nodes[4]
			} else {
				return &nodes[3]
			}
		case '[':
			writer.WriteString("$$")
			return &nodes[6]
		default:
			writer.WriteByte(ch)
			return &nodes[0]
		}
	}

	nodes[1].handle = func(ch byte) *node {
		switch ch {
		case '(':
			writer.WriteByte(ch)
			cnt = 1
			return &nodes[2]
		case ')':
			writer.WriteByte('$')
			return &nodes[0]
		default:
			writer.WriteByte(ch)
			return &nodes[1]
		}
	}

	nodes[2].handle = func(ch byte) *node {
		writer.WriteByte(ch)
		switch ch {
		case '(':
			cnt++
			return &nodes[2]
		case ')':
			cnt--
			if cnt == 0 {
				return &nodes[1]
			} else {
				return &nodes[2]
			}
		default:
			return &nodes[2]
		}
	}

	nodes[3].handle = func(ch byte) *node {
		switch ch {
		case '$':
			writer.WriteByte(ch)
			return &nodes[0]
		default:
			writer.WriteByte(ch)
			return &nodes[3]
		}
	}

	nodes[4].handle = func(ch byte) *node {
		writer.WriteByte(ch)
		switch ch {
		case '$':
			b, _ := reader.Peek(1)
			if b[0] == '$' {
				reader.ReadByte()
				writer.WriteByte('$')
				return &nodes[0]
			} else {
				return &nodes[4]
			}
		default:
			return &nodes[4]
		}
	}

	// nodes[5] is redundant.

	nodes[6].handle = func(ch byte) *node {
		switch ch {
		case '[':
			writer.WriteByte(ch)
			cnt = 1
			return &nodes[7]
		case ']':
			writer.WriteString("$$")
			return &nodes[0]
		default:
			writer.WriteByte(ch)
			return &nodes[6]
		}
	}

	nodes[7].handle = func(ch byte) *node {
		writer.WriteByte(ch)
		switch ch {
		case '[':
			cnt++
			return &nodes[7]
		case ']':
			cnt--
			if cnt == 0 {
				return &nodes[6]
			} else {
				return &nodes[7]
			}
		default:
			return &nodes[7]
		}
	}

	p := nodes[0]

	for {
		ch, err := reader.ReadByte()
		if err != nil {
			if err.Error() == "EOF" {
				break
			} else {
				log.Fatal(err)
			}
		}
		p = *p.handle(ch)
	}
}
