package main

import (
	"fmt"
	"log"
	"regexp"
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

func ReadAndSave() { //读取数据存入数据库
	data := ""
	re := regexp.MustCompile(`#\{(.*?)\}#`)
	for {
		// time.Sleep(1 * time.Second)
		data += Read()
		matches := re.FindStringSubmatch(data)
		if len(matches) > 1 {
			// log.Println("Extracted text:", matches[1]) // matches[1]是第一个括号中的内容
			fmt.Println(data)
			data = ""
		}
		// } else {
		// 	// log.Println("No match found")
		// }
	}
}

func readd() {
	for {
		fmt.Println(Read())
	}
}

func main() {
	Init("/dev/pts/10", 9600)
	// go ReadAndSave()
	// count := 1
	go readd()
	for {
		// rand.Seed(time.Now().UnixNano())
		// count = rand.Intn(15) + 15
		// data := fmt.Sprintf(`#{{ "stat": [{ "name": "Living Room", "temperature": %d, "humidity": %d , "lights": [101, 2, 3, 4], "curtains": [3, 101, 5, 6], "sockets": [true, true, false, true] }, { "name": "Kitchen", "temperature": %d, "humidity": %d, "lights": [7, 8, 9], "curtains": [10, 11, 12], "sockets": [false, true, false] }, { "name": "Bedroom", "temperature": %d, "humidity": %d, "lights": [13, 14, 15], "curtains": [16, 17, 18], "sockets": [true, false, true] }] }}#`, count, count, count, count, count, count)
		// count++
		data := "#{BedRoom_26_70_50,100_100,20_1,1,1,0,1,1,1,1}#"
		SerialPort.Write([]byte(data))
		time.Sleep(time.Second * 1)
	}
}
