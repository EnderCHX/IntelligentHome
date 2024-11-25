package serial

import (
	"IntelligentHome/config"
	"IntelligentHome/redis"
	"fmt"
	"log"
	"regexp"
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
	go ReadAndSave()
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
		time.Sleep(1 * time.Second)
		data += Read()
		matches := re.FindStringSubmatch(data)
		if len(matches) > 1 {
			// log.Println("Extracted text:", matches[1]) // matches[1]是第一个括号中的内容
			time_stamp := time.Now().Unix()
			save := fmt.Sprintf("{\"time_stamp\":%d, %s}", time_stamp, matches[1][1:len(matches[1])-1])
			// log.Println(save)
			redis.ZAdd("ihome", float64(time_stamp), save)
			data = ""
		}
		// } else {
		// 	// log.Println("No match found")
		// }
	}
}

func SendCommand(cmd string) {
	_, err := SerialPort.Write([]byte(cmd))
	if err != nil {
		log.Println(err)
	}
}
