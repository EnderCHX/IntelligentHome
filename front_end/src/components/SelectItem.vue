<template>
    <a-select v-model:value="value" show-search placeholder="Select a room" style="width: 200px" :options="options"
        :filter-option="filterOption" @focus="handleFocus" @blur="handleBlur" @change="handleChange"></a-select>
</template>
<script setup>
import { ref } from 'vue';
const options = ref([
    {
        value: 'jack',
        label: 'Jack',
    },
    {
        value: 'lucy',
        label: 'Lucy',
    },
    {
        value: 'tom',
        label: 'Tom',
    },
]);
const handleChange = value => {
    console.log(`selected ${value}`);
};
const handleBlur = () => {
    console.log('blur');
};
const handleFocus = () => {
    console.log('focus');
};
const filterOption = (input, option) => {
    return option.value.toLowerCase().indexOf(input.toLowerCase()) >= 0;
};
const value = ref(undefined);
</script>