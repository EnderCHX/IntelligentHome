import { createApp } from 'vue'
import App from './App.vue'


// 通用字体
import 'vfonts/Lato.css'
// 等宽字体
import 'vfonts/FiraCode.css'

import router from './router';
import naive from 'naive-ui';




createApp(App).use(naive).use(router).mount('#app')
