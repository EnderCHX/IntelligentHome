package api

import (
	"IntelligentHome/homecontrol"
	"IntelligentHome/redis"
	"encoding/json"
	"net/http"
	"strconv"

	"github.com/gin-gonic/gin"
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

type Stats struct {
	TimeStamp int64              `json:"time_stamp"`
	Stat      []homecontrol.Room `json:"stat"`
}

func GetStat(c *gin.Context) {
	countStr := c.DefaultQuery("count", "1")
	count, _ := strconv.Atoi(countStr)
	res, err := redis.ZRevRangeByScoreWithScores(int64(count))
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
	homecontrol.SetLights()
	homecontrol.SetCurtains()
	homecontrol.SetSockets()
	c.JSON(http.StatusOK, gin.H{
		"stat":    homecontrol,
		"message": "success",
	})
}
