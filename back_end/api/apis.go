package api

import (
	"IntelligentHome/homecontrol"
	"IntelligentHome/redis"
	"encoding/json"
	"log"
	"net/http"
	"strconv"
	"time"

	"github.com/gin-gonic/gin"
	"github.com/gorilla/websocket"
)

func Cors() gin.HandlerFunc { //跨域中间件
	return func(c *gin.Context) {
		method := c.Request.Method
		origin := c.Request.Header.Get("Origin")
		if origin != "" {
			c.Header("Access-Control-Allow-Origin", "*") // 可将将 * 替换为指定的域名
			c.Header("Access-Control-Allow-Methods", "POST, GET, OPTIONS, PUT, DELETE, UPDATE")
			c.Header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept, Authorization")
			c.Header("Access-Control-Expose-Headers", "Content-Length, Access-Control-Allow-Origin, Access-Control-Allow-Headers, Cache-Control, Content-Language, Content-Type")
			c.Header("Access-Control-Allow-Credentials", "true")
		}
		if method == "OPTIONS" {
			c.AbortWithStatus(http.StatusNoContent)
		}
		c.Next()
	}
}

type WsMessage struct {
	Type string `json:"type"`
	Data int    `json:"data"`
	Else string `json:"else"`
}

func WebSocketHandler(c *gin.Context) {
	// 获取WebSocket连接
	wsUpgrader := websocket.Upgrader{
		HandshakeTimeout: time.Second * 10,
		ReadBufferSize:   1024,
		WriteBufferSize:  1024,
		CheckOrigin: func(r *http.Request) bool {
			return true
		},
	}
	ws, err := wsUpgrader.Upgrade(c.Writer, c.Request, nil)
	if err != nil {
		log.Println(err)
		return
	}
	defer ws.Close()

	// 处理WebSocket消息
	for {
		messageType, p, err := ws.ReadMessage()
		if err != nil {
			log.Println(err)
			return
		}
		switch messageType {
		case websocket.TextMessage:
			// log.Printf("处理文本消息, %s\n", string(p))
			var wsMessage WsMessage
			json.Unmarshal(p, &wsMessage)
			switch string(wsMessage.Type) {
			case "count":
				data, err := GetDataJson(wsMessage.Data)
				if err != nil {
					ws.WriteMessage(websocket.TextMessage, []byte(err.Error()))
				}
				message, err := json.Marshal(data)
				if err != nil {
					ws.WriteMessage(websocket.TextMessage, []byte(err.Error()))
				}
				ws.WriteMessage(websocket.TextMessage, message)
			case "control":
				homecontrol := homecontrol.RoomControl{}
				json.Unmarshal([]byte(wsMessage.Else), &homecontrol)
				homecontrol.SetLights()
				homecontrol.SetCurtains()
				homecontrol.SetSockets()
				message, err := json.Marshal(homecontrol)
				if err != nil {
					ws.WriteMessage(websocket.TextMessage, []byte(err.Error()))
				}
				ws.WriteMessage(websocket.TextMessage, message)
			default:
				ws.WriteMessage(websocket.TextMessage, []byte(p))
			}
		case websocket.BinaryMessage:
			log.Println("处理二进制消息")
		case websocket.CloseMessage:
			log.Println("关闭websocket连接")
			return
		case websocket.PingMessage:
			log.Println("处理ping消息")
			ws.WriteMessage(websocket.PongMessage, []byte("ping"))
		case websocket.PongMessage:
			log.Println("处理pong消息")
			ws.WriteMessage(websocket.PongMessage, []byte("pong"))
		default:
			log.Printf("未知消息类型: %d\n", messageType)
			return
		}
	}

}

type Stats struct {
	TimeStamp int64              `json:"time_stamp"`
	Stat      []homecontrol.Room `json:"stat"`
}

func GetStat(c *gin.Context) {
	countStr := c.DefaultQuery("count", "1")
	count, _ := strconv.Atoi(countStr)
	data, err := GetDataJson(count)
	if err != nil {
		c.JSON(http.StatusOK, gin.H{
			"error": err.Error(),
		})
		return
	}
	c.JSONP(200, gin.H{
		"stats": data,
	})
}

func StatControl(c *gin.Context) {
	homecontrol := homecontrol.RoomControl{}
	c.BindJSON(&homecontrol)
	log.Println(homecontrol)
	homecontrol.SetLights()
	homecontrol.SetCurtains()
	homecontrol.SetSockets()
	c.JSON(http.StatusOK, gin.H{
		"stat":    homecontrol,
		"message": "success",
	})
}

func GetDataJson(count int) ([]Stats, error) {
	res, err := redis.ZRevRangeByScoreWithScores(int64(count))
	if err != nil {
		return nil, err
	}
	dataStr := "[ "
	for _, v := range res {
		vMember := v.Member.(string)
		dataStr += "{"
		dataStr += "\"time_stamp\":" + strconv.FormatInt(int64(v.Score), 10) + ","
		dataStr += vMember[1:len(vMember)-1] + "} ,"
	}
	dataStr = dataStr[:len(dataStr)-1] + " ]"
	// log.Println(dataStr)
	var data []Stats
	json.Unmarshal([]byte(dataStr), &data)
	return data, nil
}
