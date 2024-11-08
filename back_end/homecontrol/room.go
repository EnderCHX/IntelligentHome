package homecontrol

type Room struct {
	Name        string  `json:"name"`
	Temperature float64 `json:"temperature"`
	Humidity    float64 `json:"humidity"`
	LightsOn    []bool  `json:"lights"`
	CurtainsON  []bool  `json:"curtains"`
}
