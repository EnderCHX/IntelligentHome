package api

import (
	"IntelligentHome/config"

	"github.com/gin-gonic/gin"
)

func RunApi() {
	host := config.ConfigContext.ApiConfig.Host
	port := config.ConfigContext.ApiConfig.Port
	url := host + ":" + port
	mode := config.ConfigContext.ApiConfig.Mode
	gin.SetMode(mode)
	r := gin.Default()
	r.Use(Cors())

	r.GET("/ping", func(c *gin.Context) {
		c.JSON(200, gin.H{
			"message": "pong",
		})
	})

	r.GET("/stat", GetStat)

	r.POST("/stat/control", StatControl)

	r.GET("/ws", WebSocketHandler)

	r.Run(url)
}
