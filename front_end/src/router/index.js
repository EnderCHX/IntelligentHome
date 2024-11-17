import {createRouter, createMemoryHistory} from 'vue-router'
import Home from '../views/Home.vue'
import About from '../views/About.vue'
import History from '../views/History.vue'

const routes = [
    { 
        path: "/",
        component: Home, 
        meta: { 
            title: "智能家居管理系统"
        }
    },
    { 
        path: "/history",
        component: History,
        meta: { 
            title: "温湿度曲线"
        }
    },
    { 
        path: "/about",
        component: About,
        meta: { 
            title: "关于"
        }
    },
]

const router = createRouter(
    {
        history: createMemoryHistory(),
        routes
    }
)

router.beforeEach((to, from, next) => {
    if (to.meta.title) {
        document.title = to.meta.title
    }
    next()
})

export default router
