package homecontrol

type Room struct {
	Name        string  `json:"name"`
	Temperature float64 `json:"temperature"`
	Humidity    float64 `json:"humidity"`
	LightsOn    []int   `json:"lights"`   //0-100光照强度，大于100为自动调节
	CurtainsON  []int   `json:"curtains"` //0-100窗帘打开程度
	SocketsOn   []bool  `json:"sockets"`
	Fans        []bool  `json:"fans"`
}
