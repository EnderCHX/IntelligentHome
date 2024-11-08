package compim

import (
	"log"
	"time"

	"github.com/tarm/serial"
)

func Read(name string, baud int, stop string) string {
	c := &serial.Config{Name: name, Baud: baud, ReadTimeout: time.Second * 5}
	s, err := serial.OpenPort(c)
	if err != nil {
		log.Fatal(err)
	}

	read := ""

	buf := make([]byte, 128)
	for {
		n, err := s.Read(buf)
		if err != nil {
			log.Fatal(err)
		}
		if n == 0 {
			continue
		}
		if string(buf[:n]) == stop {
			break
		}
		read = read + string(buf[:n])
	}
	return read
}
