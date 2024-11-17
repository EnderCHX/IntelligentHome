<script setup>
import { ref, onMounted } from 'vue';
import RoomItem from '@/components/RoomItem.vue';

const stat = ref([]);
const loading = ref(true);
const active = ref(true);
let intervalId = null;

let socket = new WebSocket('ws://127.0.0.1:1314/ws');
let socket1 = new WebSocket('ws://127.0.0.1:1314/ws');

socket.onopen = async () => {
    console.log('WebSocket connected');
};

socket.onmessage = async (event) => {
    let json = JSON.parse(event.data);
    // console.log(json[0].stat);
    const data = json[0].stat;
    // console.log(data);
    stat.value = data;
}

socket.onerror = (error) => {
    console.error('WebSocket error:', error);
    loading.value = true;
};

socket.onclose = (event) => {
    console.log('WebSocket closed:', event);
    loading.value = true;
};



async function GetData() {
    const url = 'http://127.0.0.1:1314/stat';
    try {
        const response = await fetch(url);
        const json = await response.json();

        return json.stats[0].stat; // 返回数据数组
    } catch (error) {
        console.error('Error in GetData:', error);
        return []; // 请求失败返回空数组
    }
}

async function fetchData() {
    const data = await GetData();
    stat.value = data;
    loading.value = false;
}

function socketSend() {
    socket.send(`{"type": "count", "data": 1}`)
}

onMounted(async () => {
    await fetchData();
    // socket.send(`{"type": "count", "data": 1}`)
    console.log(stat.value);
    loading.value = false;
    
    intervalId = setInterval(socketSend, 1000);
});

onUnmounted(() => {
    clearInterval(intervalId);
    socket.close();
});
</script>

<template>
    <div class="room-container">
        <RoomItem class="room-item" v-for="(room, index) in stat" :key="index" :stat="room" 
            :websocket="socket1"
            :loading="loading"
            :active="active" />
    </div>
</template>

<style scoped>
.room-container {
    flex-wrap: wrap;
    display: flex;
}

.room-item {
    flex: 1 1 48%;
    padding: 1%;
}
</style>
