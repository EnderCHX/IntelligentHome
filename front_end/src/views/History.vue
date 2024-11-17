<script setup>
import ChartItem from '@/components/ChartItem.vue';
import { onMounted } from 'vue';

const fetchData = async () => {
    let url = "http://127.0.0.1:1314/stat?count=300"
    let response = await fetch(url);
    let data = await response.json();
    return data;
}

const data = ref([{
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
}]);
let intervalId = null;

let websocket = new WebSocket("ws://127.0.0.1:1314/ws");

websocket.onmessage = function (event) {
    // console.log("event data", event.data);
    data.value = JSON.parse(event.data);
    // console.log("datavalue", data.value);
}
websocket.onerror = function (event) {
    console.log("websocket error", event);
}
websocket.onclose = function (event) {
    console.log("websocket close", event);
}
function socketSend() {
    websocket.send(`{"type": "count", "data": 300}`)
}

onMounted(async () => {
    let _data = await fetchData();
    console.log("data", _data.stats);
    data.value = _data.stats;
    console.log("datavalue", data.value);
    intervalId = setInterval(socketSend, 1000);
})

onUnmounted(() => {
    clearInterval(intervalId);
    socketSend();
})



</script>

<template>
    <n-layout>
        <ChartItem :index="index" :stats="data" v-for="(item, index) in data[0].stat" />
    </n-layout>
</template>