package serial

import (
	"IntelligentHome/config"
	"IntelligentHome/redis"
	"fmt"
	"log"
	"regexp"
	"strings"
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
	// go ReadAndSave()
	go ReadAndSaveLite()
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

func ReadAndSaveLite() {
	data := ""
	re := regexp.MustCompile(`#\{(.*?)\}#`)
	for {
		time.Sleep(1 * time.Second)
		data += Read()
		matches := re.FindStringSubmatch(data)
		if len(matches) > 1 {
			result := strings.Split(matches[1], "_")
			if len(result) > 0 {
				log.Println(result)
				time_stamp := time.Now().Unix()
				roomName := result[0]
				temperature := result[1]
				humidity := result[2]
				lights := result[3]
				curtains := result[4]
				socketsstr := strings.Split(result[5], ",")
				sockets := ""
				for i, socketstr := range socketsstr {
					if socketstr == "1" {
						sockets += "true"
					} else if socketstr == "0" {
						sockets += "false"
					}
					if i < len(socketsstr)-1 {
						sockets += ","
					}
				}
				save := fmt.Sprintf("{\"time_stamp\":%d, \"stat\": [ { \"name\": \"%s\", \"temperature\": %s, \"humidity\": %s, \"lights\": [ %s ], \"curtains\": [ %s ], \"sockets\": [ %s ] } ] } ", time_stamp, roomName, temperature, humidity, lights, curtains, sockets)
				redis.ZAdd("ihome", float64(time_stamp), save)
				data = ""
			}
		}
	}
}

func SendCommand(cmd string) {
	_, err := SerialPort.Write([]byte(cmd))
	if err != nil {
		log.Println(err)
	}
}
