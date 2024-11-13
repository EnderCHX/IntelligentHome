package compim

import (
	"IntelligentHome/config"
	"log"
	"time"

	"github.com/tarm/serial"
)

var SerialPort *serial.Port

func Init() {
	name := config.ConfigContext.ChuanKouConfig.Name
	baud := config.ConfigContext.ChuanKouConfig.Baud
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
