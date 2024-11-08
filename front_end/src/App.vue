<script setup>
import HelloWorld from './components/HelloWorld.vue'
import DocumentationIcon from './components/icons/IconDocumentation.vue'
import WelcomeItem from './components/WelcomeItem.vue'
import ToolingIcon from './components/icons/IconTooling.vue'
import EcosystemIcon from './components/icons/IconEcosystem.vue'
import CommunityIcon from './components/icons/IconCommunity.vue'
import SupportIcon from './components/icons/IconSupport.vue'

import { defineAsyncComponent, ref, onMounted } from 'vue'
import axios from 'axios'

async function GetRoomStat() {
    let url = "http://localhost:8080/stat"
    let data = await axios({
        url: url,
        method: 'GET',
    })
    rooms.value = data.data.stat
    console.log(rooms.value)
}



const rooms = ref([])
GetRoomStat()

console.log(rooms.value)
</script>

<template>
    <header>
        <img alt="Vue logo" class="logo" src="./assets/logo.svg" width="125" height="125" />

        <div class="wrapper">
            <HelloWorld msg="智能家居管理系统" />
        </div>
    </header>

    <main>
        <WelcomeItem>
            <template #icon>
                <DocumentationIcon />
            </template>
            <template #heading>房间</template>
            <div v-for="room of rooms">
                <hr>
                <p>房间名: {{ room.Name }}</p>
                <p>温度: {{ room.Temperature }}</p>
                <p>湿度: {{ room.Humidity }}</p>
                <p>光照: <span v-for="light of room.LightsOn">{{ light }} <button>开/关</button>,</span></p>
                <p>窗帘: <span v-for="curtain of room.CurtainsON">{{ curtain }} <button>开/关</button>,</span></p>
                <hr>
            </div>
        </WelcomeItem>
    </main>
</template>

<style scoped>
header {
    line-height: 1.5;
}

.logo {
    display: block;
    margin: 0 auto 2rem;
}

@media (min-width: 1024px) {
    header {
        display: flex;
        place-items: center;
        padding-right: calc(var(--section-gap) / 2);
    }

    .logo {
        margin: 0 2rem 0 0;
    }

    header .wrapper {
        display: flex;
        place-items: flex-start;
        flex-wrap: wrap;
    }
}
</style>