import {createRouter, createMemoryHistory} from 'vue-router'
import Home from '../views/Home.vue'

const router = createRouter(
    {
        history: createMemoryHistory(),
        routes:[
            { path: "/", component: Home },
            // { path: "rooms", component: Rooms }  
        ]
    }
)

export default router
