package homecontrol

import (
	"IntelligentHome/serial"
	"fmt"
	"log"
)

type RoomControl struct {
	Name       string `json:"name"`
	LightsOn   []int  `json:"lights"`   //0-100光照强度，大于100为自动调节
	CurtainsON []int  `json:"curtains"` //0-100窗帘打开程度
	SocketsOn  []bool `json:"sockets"`
}

func (c *RoomControl) SetLights() {
	cmd := "#{set " + c.Name + " lights "
	for _, light := range c.LightsOn {
		cmd += fmt.Sprintf("%d ", light)
	}
	cmd += "}#"
	serial.SendCommand(cmd)
}

func (c *RoomControl) SetCurtains() {
	cmd := "#{set " + c.Name + " curtains "
	for _, curtain := range c.CurtainsON {
		cmd += fmt.Sprintf("%d ", curtain)
	}
	cmd += "}#"
	serial.SendCommand(cmd)
}

func (c *RoomControl) SetSockets() {
	cmd := "#{set " + c.Name + " sockets "
	for _, socket := range c.SocketsOn {
		if socket {
			cmd += "1 "
		} else {
			cmd += "0 "
		}
	}
	cmd += "}#"
	serial.SendCommand(cmd)
}

func (c *RoomControl) SetAll() {
	cmd := ""
	for i, light := range c.LightsOn {
		cmd += fmt.Sprintf("%d", light)
		if i < len(c.LightsOn)-1 {
			cmd += ","
		}
	}
	cmd += "_"
	for i, curtain := range c.CurtainsON {
		cmd += fmt.Sprintf("%d", curtain)
		if i < len(c.CurtainsON)-1 {
			cmd += ","
		}
	}
	cmd += "_"
	for i, socket := range c.SocketsOn {
		if socket {
			cmd += "1"
		} else {
			cmd += "0"
		}
		if i < len(c.SocketsOn)-1 {
			cmd += ","
		}
	}
	cmd += "\r\n"
	log.Println(cmd)
	serial.SendCommand(cmd)
}

/*
	#{set room1 lights 100 100 100 100}#
*/
