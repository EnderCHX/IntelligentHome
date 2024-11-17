<template>
    <n-space vertical>
        <n-card>
            <template #header>
                <n-skeleton v-if="loading" text width="60%" />
                <template v-else>
                    {{ stat.name }}
                </template>
            </template>
            <n-skeleton v-if="loading" text :repeat="6" />
            <template v-else>
                <n-space>
                    <n-icon size="40">
                        <TachometerAlt />
                    </n-icon>

                    <RouterLink :to="`/history`" style="text-decoration: none"><n-button >点击查看温湿度曲线</n-button></RouterLink>
                </n-space>

                <n-divider />
                <n-row>
                    <n-col :span="5">
                        <n-statistic label="温度">
                            <n-number-animation :active="active" ref="numberAnimationInstRef" :from="stat.temperature"
                                :to="stat.temperature" />
                            <n-icon>
                                <TemperatureCelsius />
                            </n-icon>
                        </n-statistic>
                    </n-col>
                    <n-col :span="5">
                        <n-statistic label="湿度">
                            <n-number-animation :active="active" ref="numberAnimationInstRef" :from="stat.humidity"
                                :to="stat.humidity" />
                            <n-icon>
                                <Humidity />
                            </n-icon>
                        </n-statistic>
                    </n-col>
                </n-row>
                <n-divider />

                <n-row v-for="(light, index) in stat.lights" :key="index">
                    <n-icon size="40" :depth="5 - (lightvalue[index] - lightvalue[index] % 20) / 20">
                        <Lightbulb />
                    </n-icon>
                    <n-statistic v-if="light > 100">灯{{ index + 1 }}：AUTO</n-statistic>
                    <n-statistic v-else>灯{{ index + 1 }}：{{ light }}%</n-statistic>
                    <n-slider v-model:value="lightvalue[index]" :step="1" :max="120" :marks="marks"
                        @dragend="lightDragEnd(stat.name, lightvalue, stat.lights.length)" />
                    <n-divider dashed v-if="index < stat.lights.length - 1" />
                </n-row>
                <n-divider />

                <n-row v-for="(curtain, index) in stat.curtains" :key="index">
                    <n-icon size="40" :depth="5 - (curtainvalue[index] - curtainvalue[index] % 20) / 20">
                        <WindowRound />
                    </n-icon>
                    <n-statistic v-if="curtain > 100">窗帘{{ index + 1 }}：AUTO</n-statistic>
                    <n-statistic v-else>窗帘{{ index + 1 }}：{{ curtain }}%</n-statistic>
                    <n-slider v-model:value="curtainvalue[index]" :step="1" :max="120" :marks="marks"
                        @dragend="curtainDragEnd(stat.name, curtainvalue, stat.curtains.length)" />
                    <n-divider dashed v-if="index < stat.curtains.length - 1" />
                </n-row>
                <n-divider />
                <n-row v-for="(socket_, index) in stat.sockets" :key="index">
                    <n-icon size="40" :color="socketvalue[index] ? 'green' : 'gray'">
                        <n-switch 
                            v-model:value="socketvalue[index]"
                            @update:value="changeSocket(stat.name, socketvalue, $event)" />
                    </n-icon>
                    <n-statistic>插座{{ index + 1 }}</n-statistic>
                    <n-divider dashed v-if="index < stat.sockets.length - 1" />
                </n-row>
            </template>
        </n-card>
    </n-space>
</template>

<script setup>
import { ref } from 'vue';
import { Humidity } from '@vicons/carbon';
import { TemperatureCelsius } from '@vicons/tabler';
import { TachometerAlt, Lightbulb } from '@vicons/fa';
import { WindowRound } from '@vicons/material';


// Props
const props = defineProps({
    websocket: {
        type: WebSocket,
        required: true,
    },
    active: {
        type: Boolean,
        default: true,
    },
    loading: {
        type: Boolean,
        default: true,
        required: true,
    },
    stat: {
        type: Object,
        default: () => ({
            name: 'Room Name',
            temperature: 26,
            humidity: 50,
            lights: [0, 0, 0],
            curtains: [0, 0, 0],
            sockets: [false, false, false],
        }),
    },
});

console.log(props.stat);

const lightvalue = props.stat.lights
const curtainvalue = props.stat.curtains
const socketvalue = props.stat.sockets

let socket1 = props.websocket;

socket1.onopen = async () => {
    console.log('WebSocket connected');
};

socket1.onmessage = async (event) => {
    let json = JSON.parse(event.data);
    // console.log(json[0].stat);
    const data = json;
    console.log(data);

}

socket1.onerror = (error) => {
    console.error('WebSocket error:', error);
};

socket1.onclose = (event) => {
    console.log('WebSocket closed:', event);
};


function DragStart(e) {
    console.log('dragstart', e);
}

async function lightDragEnd(name, lightvalue, len) {
    console.log('dragend', name, lightvalue.map((v) => v), len);
    lightvalue.forEach((v, i) => {
        if (v > 100) {
            lightvalue[i] = 120;
        }
    });
    console.log(lightvalue.map((v) => v));
    await changeStat(name);
}

async function curtainDragEnd(name, curtainvalue, len) {
    console.log('dragend', name, curtainvalue.map((v) => v), len);
    curtainvalue.forEach((v, i) => {
        if (v > 100) {
            curtainvalue[i] = 120;
        }
    });

    console.log(curtainvalue.map((v) => v));
    await changeStat(name);
}

async function changeSocket(name, socketvalue, value) {
    console.log('changeSocket', name, socketvalue.map((v) => v), value);
    await changeStat(name);
}

async function changeStat(name) {
    let data = {
        name: name,
        lights: lightvalue.map((v) => v),
        curtains: curtainvalue.map((v) => v),
        sockets: socketvalue.map((v) => v),
    };
    console.log('changeStat', data);
    let datasend = {
        type: 'control',
        else: JSON.stringify(data),
    }
    // await fetch('http://127.0.0.1:1314/stat/control', {
    //     method: 'POST',
    //     headers: {
    //         'Content-Type': 'application/json',
    //     },
    //     body: JSON.stringify(data),
    // })
    //     .then(response => response.json())
    //     .then(data => {
    //         console.log('changeStat', data);
    //     })
    //     .catch(error => {
    //         console.error('changeStat', error);
    //     });
    socket1.send(JSON.stringify(datasend));
}

const marks = {
    0: '关闭',
    100: '最大',
    120: '自动',
};
</script>
