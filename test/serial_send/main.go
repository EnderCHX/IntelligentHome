package main

import (
	"fmt"
	"log"
	"time"

	"github.com/tarm/serial"
)

var SerialPort *serial.Port

func Init(name string, baud int) {
	var err error
	c := &serial.Config{Name: name, Baud: baud, ReadTimeout: time.Second * 5}
	SerialPort, err = serial.OpenPort(c)
	if err != nil {
		log.Fatal(err)
	} else {
		log.Printf("Serial port %s opened successfully.\n", name)
	}
}

func Read() string {

	buf := make([]byte, 128)
	n, err := SerialPort.Read(buf)
	if err != nil {
		log.Fatal(err)
	}

	return string(buf[:n])

}

func main() {
	Init("COM1", 9600)
	count := 1
	for {
		data := fmt.Sprintf(`#{{ "stat": [{ "name": "Living Room", "temperature": %d, "humidity": 50.000000, "lights": [101, 2, 3, 4], "curtains": [3, 101, 5, 6], "sockets": [false, true, false, true] }, { "name": "Kitchen", "temperature": %d, "humidity": 60.000000, "lights": [7, 8, 9], "curtains": [10, 11, 12], "sockets": [false, true, false] }, { "name": "Bedroom", "temperature": %d, "humidity": 40.000000, "lights": [13, 14, 15], "curtains": [16, 17, 18], "sockets": [true, false, true] }] }}#`, count, count, count)
		count++
		SerialPort.Write([]byte(data))
		time.Sleep(time.Second * 1)
	}
}
