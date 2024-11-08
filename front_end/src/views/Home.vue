<script setup>
import HelloWorld from '../components/HelloWorld.vue'
import DocumentationIcon from '../components/icons/IconDocumentation.vue'
import WelcomeItem from '../components/WelcomeItem.vue'
import ToolingIcon from '../components/icons/IconTooling.vue'
import EcosystemIcon from '../components/icons/IconEcosystem.vue'
import CommunityIcon from '../components/icons/IconCommunity.vue'
import SupportIcon from '../components/icons/IconSupport.vue'
import NewSelect from '../components/SelectItem.vue'

import { defineAsyncComponent, ref, onMounted } from 'vue'
import axios from 'axios'

const rooms = ref([])
const selected = ref('')

async function GetRoomStat() {
    let url = "http://localhost:8080/stat"
    let data = await axios({
        url: url,
        method: 'GET',
    })
    rooms.value = data.data.stat
    console.log(rooms.value)
}

function ifShow(a, b) {
    console.log(a, b)
    return a == b ? true : false
}


GetRoomStat()



console.log(rooms.value)

</script>

<template>
    <header>
        <img alt="Vue logo" class="logo" src="../assets/logo.svg" width="125" height="125" />

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
            <select v-model="selected">
                <option v-for="room of rooms">{{ room.name }}</option>
            </select>
            <div v-for="room of rooms">
                <div v-show="ifShow(selected, room.name)">
                    <hr>
                    <p>房间名: {{ room.name }}</p>
                    <p>温度: {{ room.temperature }}</p>
                    <p>湿度: {{ room.humidity }}</p>
                    <p>光照: <span v-for="light of room.lights">{{ light }} <button>开/关</button>,</span></p>
                    <p>窗帘: <span v-for="curtain of room.curtains">{{ curtain }} <button>开/关</button>,</span></p>
                    <hr>
                </div>
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



.option {
    /*用div的样式代替select的样式*/
    margin: 100px;
    width: 140px;
    height: 40px;
    /*border-radius: 5px;*/
    /*盒子阴影修饰作用,自己随意*/
    /* box-shadow: 0 0 5px #ccc;*/
    border: 1px solid #cccccc;
    position: relative;
}

.option select {
    /*清除select的边框样式*/
    border: none;
    /*清除select聚焦时候的边框颜色*/
    outline: none;
    /*将select的宽高等于div的宽高*/
    width: 100%;
    height: 40px;
    line-height: 40px;
    /*隐藏select的下拉图标*/
    appearance: none;
    -webkit-appearance: none;
    -moz-appearance: none;
    /*通过padding-left的值让文字居中*/
    padding-left: 20px;
}

/*使用伪类给select添加自己想用的图标*/
.option:after {
    content: "";
    width: 14px;
    height: 8px;
    background: url(../assets/arrow-down.png) no-repeat center;
    /*通过定位将图标放在合适的位置*/
    position: absolute;
    right: 20px;
    top: 41%;
    /*给自定义的图标实现点击下来功能*/
    pointer-events: none;
}
</style>