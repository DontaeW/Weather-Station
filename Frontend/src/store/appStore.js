import {defineStore} from 'pinia'
import {ref} from 'vue'


export const useAppStore =  defineStore('app', ()=>{

    /*  
    The composition API way of defining a Pinia store
    ref() s become state properties
    computed() s become getters
    function() s become actions  
    */ 

    const getAllInRange = async (start,end)=>{
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS
        const controller = new AbortController();
        const signal = controller.signal;
        const id = setTimeout(()=>{controller.abort()},60000);
        const URL = `/api/weatherstation/get/${start}/${end}`;
        try {
            const response = await fetch(URL,{ method: 'GET', signal: signal });
            if (response.ok){
                const data = await response.json();
                let keys = Object.keys(data);
                if(keys.includes("status")){
                    if(data["status"] == "found" ){
                        console.log(data["data"] )
                        return data["data"];
            }
            if(data["status"] == "failed"
                ){
                console.log("getAllInRange returned no data");
            }
        }
    }
    else{
        const data = await response.text();
        console.log(data);
        }
    }
    catch(err){
            
        console.error('getAllInRange error: ', err.message);
    }
    return []
}

const gettemperatureMMAR = async (start, end) => {
    // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS
    const controller = new AbortController();
    const signal = controller.signal;
    const id = setTimeout(() => { controller.abort() }, 60000);
    const URL = `/api/mmar/temperature/${start}/${end}`;
    try {
        const response = await fetch(URL, { method: 'GET', signal: signal });
        if (response.ok) {
            const data = await response.json();
            let keys = Object.keys(data);
            if (keys.includes("status")) {
                if (data["status"] == "found") {
                    console.log(data["data"]);
                    return data["data"];
                }
                if (data["status"] == "failed"
                ) {
                    console.log("gettemperatureMMAR returned no data");
                }
            }
        }
        else {
            const data = await response.text();
            console.log(data);
        }
    }
    catch (err) {
        console.error('gettemperatureMMAR error: ', err.message);
    }
    return []
}



const gethumidityMMAR = async (start, end) => {
    // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS
    const controller = new AbortController();
    const signal = controller.signal;
    const id = setTimeout(() => { controller.abort() }, 60000);
    const URL = `/api/mmar/humidity/${start}/${end}`;
    try {
        const response = await fetch(URL, { method: 'GET', signal: signal });
        if (response.ok) {
            const data = await response.json();
            let keys = Object.keys(data);
            if (keys.includes("status")) {
                if (data["status"] == "found") {
                    console.log(data["data"]);
                    return data["data"];
                }
                if (data["status"] == "failed"
                ) {
                    console.log("gethumidityMMAR returned no data");
                }
            }
        }
        else {
            const data = await response.text();
            console.log(data);
        }
    }
    catch (err) {
        console.error('gethumidityMMAR error: ', err.message);
    }
    return []
}


const getpressureMMAR = async (start, end) => {
    // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS
    const controller = new AbortController();
    const signal = controller.signal;
    const id = setTimeout(() => { controller.abort() }, 60000);
    const URL = `/api/mmar/Pressure/${start}/${end}`;
    try {
        const response = await fetch(URL, { method: 'GET', signal: signal });
        if (response.ok) {
            const data = await response.json();
            let keys = Object.keys(data);
            if (keys.includes("status")) {
                if (data["status"] == "found") {
                    console.log(data["data"]);
                    return data["data"];
                }
                if (data["status"] == "failed"
                ) {
                    console.log("getpressureMMAR returned no data");
                }
            }
        }
        else {
            const data = await response.text();
            console.log(data);
        }
    }
    catch (err) {
        console.error('getpressureMMAR error: ', err.message);
    }
    return []
}

const getaltitudeMMAR = async (start, end) => {
    // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS
    const controller = new AbortController();
    const signal = controller.signal;
    const id = setTimeout(() => { controller.abort() }, 60000);
    const URL = `/api/mmar/Altitude/${start}/${end}`;
    try {
        const response = await fetch(URL, { method: 'GET', signal: signal });
        if (response.ok) {
            const data = await response.json();
            let keys = Object.keys(data);
            if (keys.includes("status")) {
                if (data["status"] == "found") {
                    console.log(data["data"]);
                    return data["data"];
                }
                if (data["status"] == "failed"
                ) {
                    console.log("getaltitudeMMAR returned no data");
                }
            }
        }
        else {
            const data = await response.text();
            console.log(data);
        }
    }
    catch (err) {
        console.error('getaltitudeMMAR error: ', err.message);
    }
    return []
}

const getsoilmoistureMMAR = async (start, end) => {
    // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS
    const controller = new AbortController();
    const signal = controller.signal;
    const id = setTimeout(() => { controller.abort() }, 60000);
    const URL = `/api/mmar/Soil Moistuire/${start}/${end}`;
    try {
        const response = await fetch(URL, { method: 'GET', signal: signal });
        if (response.ok) {
            const data = await response.json();
            let keys = Object.keys(data);
            if (keys.includes("status")) {
                if (data["status"] == "found") {
                    console.log(data["data"]);
                    return data["data"];
                }
                if (data["status"] == "failed"
                ) {
                    console.log("getsoilmoistureMMAR returned no data");
                }
            }
        }
        else {
            const data = await response.text();
            console.log(data);
        }
    }
    catch (err) {
        console.error('getsoilmoistureMMAR error: ', err.message);
    }
    return []
}


const getFreqDistro = async (variable, start, end) => {
    // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS
    const controller = new AbortController();
    const signal = controller.signal;
    const id = setTimeout(() => { controller.abort() }, 60000);
    const URL = `/api/frequency/${variable}/${start}/${end}`;
    try {
        const response = await fetch(URL, { method: 'GET', signal: signal });
        if (response.ok) {
            const data = await response.json();
            let keys = Object.keys(data);
            if (keys.includes("status")) {
                if (data["status"] == "found") {
                    console.log(data["data"]);
                    return data["data"];
                }
                if (data["status"] == "failed"
                ) {
                    console.log("getFreqDistro returned no data");
                }
            }
        }
        else {
            const data = await response.text();
            console.log(data);
        }
    }
    catch (err) {
        console.error('getFreqDistro error: ', err.message);
    }
    return []
}

// STATES 



// ACTIONS


return {
    // EXPORTS	
    getAllInRange,
    gettemperatureMMAR,
    gethumidityMMAR,
    getsoilmoistureMMAR,
    getpressureMMAR,
    getaltitudeMMAR,
    getFreqDistro,
}
}, { persist: true });


