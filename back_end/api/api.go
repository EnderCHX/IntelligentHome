package api

import (
	"IntelligentHome/config"
	"net/http"

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

	const (
		StaticPath    = "./static"
		StaticWebPath = "static"
		TemplatesPath = "./templates/*"
	)
	r.LoadHTMLGlob(TemplatesPath)
	r.Static(StaticWebPath, StaticPath)

	r.StaticFile("/favicon.ico", "./static/favicon.ico")

	r.GET("/", func(c *gin.Context) {
		c.HTML(http.StatusOK, "index.html", gin.H{
			"data":   "ciallo world",
			"static": StaticWebPath,
		})
	})

	r.GET("/ping", func(c *gin.Context) {
		c.JSON(200, gin.H{
			"message": "pong",
		})
	})

	r.GET("/stat", GetStat)

	r.POST("/stat/control", StatControl)

	r.Run(url)
}
