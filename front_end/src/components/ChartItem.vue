<script setup>
import { ref, onMounted, watch, defineProps } from 'vue';
import * as echarts from 'echarts';

const tempchart = ref();
const humichart = ref();

const props = defineProps({
    index: {
        type: Number,
        default: 0,
    },
    stats: {
        type: Array,
        default: [
            {
                time_stamp: 1731753920,
                stat: [
                    {
                        name: "RoomName",
                        temperature: 26,
                        humidity: 50,
                        lights: [0, 0, 0, 0],
                        curtains: [0, 0, 0],
                        sockets: [false, true, false, true],
                    },
                ],
            },
        ],
    },
});

let myChart1;
let myChart2;
const index = props.index;

const updateChart = () => {
    const time_data = props.stats.map((item) => {
        let date = new Date(item.time_stamp * 1000);
        return date.toLocaleString();
    }).reverse();
    const temperature_data = props.stats.map((item) => item.stat[index].temperature).reverse();
    const humidity_data = props.stats.map((item) => item.stat[index].humidity).reverse();
    const temptitle = ref("温度曲线");
    const humititle = ref("湿度曲线");
    if (props.stats[0]) {
        temptitle.value = props.stats[0].stat[index].name + "温度曲线"
        humititle.value = props.stats[0].stat[index].name + "湿度曲线";
    }

    // 更新图表数据
    if (myChart1) {
        myChart1.setOption({
            title: {
                text: temptitle.value,
            },
            xAxis: {
                data: time_data,
            },
            series: [
                {
                    data: temperature_data,
                },
            ],
        });
    }
    if (myChart2) {
        myChart2.setOption({
            title: {
                text: humititle.value,
            },
            xAxis: {
                data: time_data,
            },
            series: [
                {
                    data: humidity_data,
                },
            ],
        });
    }
};

onMounted(() => {
    const temptitle = "温度曲线";
    const humititle = "湿度曲线";
    myChart1 = echarts.init(tempchart.value, null, {
        renderer: 'svg'
    });
    myChart2 = echarts.init(humichart.value, null, {
        renderer: 'svg'
    });


    myChart1.setOption({
        title: {
            text: temptitle,
        },
        xAxis: {
            type: "category",
            data: [],
        },
        yAxis: {
            type: "value",
        },
        series: [
            {
                data: [],
                type: "line",
                smooth: true,
            },
        ],
    });

    myChart2.setOption({
        title: {
            text: humititle,
        },
        xAxis: {
            type: "category",
            data: [],
        },
        yAxis: {
            type: "value",
        },
        series: [
            {
                data: [],
                type: "line",
                smooth: true,
            },
        ],
    });

    updateChart(); // 初始化时设置图表数据
});

// 监听 props.stats 的变化
watch(
    () => props.stats,
    () => {
        updateChart();
    },
    { deep: true } // 深度监听，确保嵌套数据变化也会触发更新
);
</script>

<template>
    <n-flex>
        <div ref="tempchart" style="width: 48%; height: 400px;"></div>
        <div ref="humichart" style="width: 48%; height: 400px;"></div>
    </n-flex>
</template>
