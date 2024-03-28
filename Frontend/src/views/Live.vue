<template>
  <v-container class="bg-surface mb-6" align="center">
    <v-row class="row">

   <!-- <v-row :max-width="1000"> -->
    <v-col align="start" cols="8" no-gutters>
                        
     <v-card class="mx-auto" :width="1150" :height="300" rounded="xl" align="center" justify="center" title="Temperature">
                                    
      <v-row justify="start">
      
      <v-col align="center">
        <span class="text-h2"> {{ temperature }} </span>

        <!-- <v-col>
        <span class="text-h2"> {{ humidity}} </span>
      </v-col> -->

      </v-col>
      <v-col>
              <VImg height="80" class="mb-10" :src="`https://openweathermap.org/img/wn/${cloudski}d@2x.png`" />
      </v-col>
      </v-row>
         <p class="ma-5">{{ cloudskiisss }}</p><br>
        </v-card>
        <v-btn class="mt-5" align="center"  @click="convertTo();" color="surface-variant"  variant="outlined" > Convert to {{ values }}
       </v-btn>
   </v-col>

          <v-row></v-row>
          <v-row></v-row>
  
  <v-row>

</v-row>


<v-row></v-row>
    


  <v-col cols="9">
    <figure class="highcharts-figure">
      <div id="container"></div>
        </figure>
  </v-col>
  <v-col cols="3">
    <v-card
    class="mb-5 mr-5" 
    style="max-width: 1000px"
    color="primaryContainer"
    subtitle="Altitude (m)"
  >
    <v-card-item>
      <span class="text-onPrimaryContainer text-h3">
        {{ altitude }}
      </span>
    </v-card-item>  
  </v-card>

  <v-card
    class="mb-5 mr-5" 
    style="max-width: 1000px"
    color="tertiaryContainer"
    subtitle="Pressure"
  >
    <v-card-item>
      <span class="text-onTertiaryContainer text-h3">
        {{ pressure }}
      </span>
    </v-card-item>
  </v-card>

  <v-card
    class="mb-5 mr-5" 
    style="max-width: 1000px"
    color="secondaryContainer"
    subtitle="Humidity"
  >
    <v-card-item>
      <span class="text-onSecondaryContainer text-h3">
        {{ humidity }}
      </span>
    </v-card-item>
  </v-card>


  <v-card
    class="mb-5 mr-5" 
    style="max-width: 1000px"
    color="secondaryContainer"
    subtitle="Soil Moisture"
  >
    <v-card-item>
      <span class="text-onSecondaryContainer text-h3">
        {{ soilmoisture }}
      </span>
    </v-card-item>
  </v-card>

          
        
      </v-col>
    </v-row>


    <v-row class="row" justify="start">
      <v-col class="col-9">
        <figure class="highcharts-figure">
          <div id="container1"></div>
        </figure>
      </v-col>
    </v-row>
  </v-container>
</template>

<script setup>
/** JAVASCRIPT HERE */

// IMPORTS

// Highcharts, Load the exporting module and Initialize exporting module.
import Highcharts from "highcharts";
import more from "highcharts/highcharts-more";
import Exporting from "highcharts/modules/exporting";
Exporting(Highcharts);
more(Highcharts);

import { ref,reactive,watch ,onMounted,onBeforeUnmount,computed } from "vue";  
import { useRoute ,useRouter } from "vue-router";
import { useMqttStore } from '@/store/mqttStore'; // Import Mqtt Store
// import WeatherWidget from '@/components/WeatherWidget.vue'
import { storeToRefs } from "pinia";




// VARIABLES
const router = useRouter();
const route = useRoute();
const Mqtt = useMqttStore();
const { payload, payloadTopic } = storeToRefs(Mqtt);

//CHARTS
const tempHiChart = ref(null); // Chart object
const humChart = ref(null); // Chart object


const mqtt = ref(null);
const host = ref("dbs.msjrealtms.com"); // Host Name or IP address
const port = ref(9002); // Port number
const points = ref(10); 
const cloudskiisss = ref(""); 
const shift = ref(false); 
const cloudski = ref("02"); // Weather symbol


const temperature = computed(()=>{
    if(!!payload.value){
            return `${payload.value.Temperature.toFixed(2)} °C`;}
    }
);
const pressure = computed(()=>{
    if(!!payload.value){
            return `${payload.value.Pressure.toFixed(2)} Pa`;}
    }
);
const altitude = computed(()=>{
    if(!!payload.value){
            return `${payload.value.Altitude.toFixed(2)} m`;}
    }
);
const soilmoisture = computed(()=>{
    if(!!payload.value){
            return `${payload.value.SoilMoisture.toFixed(2)}%`;}
    }
);
const humidity = computed(()=>{
    if(!!payload.value){
            return `${payload.value.Humidity.toFixed(2)}%`;}
    }
);







// FUNCTIONS

onMounted(() => {
  // THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
  CreateCharts();
  CreateCharts_2();

  Mqtt.connect(); // Connect to Broker located on the backend
  setTimeout(() => {
    // Subscribe to each topic
    Mqtt.subscribe("620157609");
    Mqtt.subscribe("620157609_sub");
  }, 3000);
});

onBeforeUnmount(() => {
  // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
  // unsubscribe from all topics
  Mqtt.unsubcribeAll();
});



const CreateCharts = async () => {
  // TEMPERATURE CHART
  tempHiChart.value = Highcharts.chart("container", {
    chart: { zoomType: "x" },
    title: { text: "Air Temperature and Heat Index Analysis", align: "left" },
    yAxis: {
      title: {
        text: "Air Temperature & Heat Index",
        style: { color: "#000000" },
      },
      labels: { format: "{value} °C" },
    },
    xAxis: {
      type: "datetime",
      title: { text: "Time", style: { color: "#000000" } },
    },
    tooltip: { shared: true },
    series: [
      {
        name: "Temperature",
        type: "spline",
        data: [],
        turboThreshold: 0,
        color: Highcharts.getOptions().colors[0],
      },
      {
        name: "Heat Index",
        type: "spline",
        data: [],
        turboThreshold: 0,
        color: Highcharts.getOptions().colors[1],
      },
    ],
  });
};

const CreateCharts_2 = async () => {
  // Humidity CHART
  humChart.value = Highcharts.chart("container1", {
    chart: { zoomType: "x" },
    title: { text: "Humidity Analysis", align: "left" },
    yAxis: {
      title: {
        text: "Humidity",
        style: { color: "#000000" },
      },
      labels: { format: "{value} %" },
    },
    xAxis: {
      type: "datetime",
      title: { text: "Time", style: { color: "#000000" } },
    },
    tooltip: { shared: true },
    series: [
      {
        name: "Humidity",
        type: "spline",
        data: [],
        turboThreshold: 0,
        color: Highcharts.getOptions().colors[0],
      },
    ],
  });
};

// COMPUTED PROPERTIES


// WATCHERS
watch(payload, (data) => {
  if (points.value > 0) {
    points.value--;
  } else {
    shift.value = true;
  }
  tempHiChart.value.series[0].addPoint(
    { y: parseFloat(data.Temperature.toFixed(2)), x: data.timestamp * 1000 },
    true,
    shift.value
  );
  tempHiChart.value.series[1].addPoint(
    { y: parseFloat(data.HeatIndex.toFixed(2)), x: data.timestamp * 1000 },
    true,
    shift.value
  );
  humChart.value.series[0].addPoint(
    { y: parseFloat(data.Humidity.toFixed(2)), x: data.timestamp * 1000 },
    true,
    shift.value
  );
});

const converttofahrenheit = (celsius) => {
    return (celsius * 9/5) + 32;
};



const values = ref("Fahrenheit"); // Temperature values
function convertTo() {
    if (this.values === "Celsius") {
        // Convert Celsius to Fahrenheit
        this.temperature = (this.temperature * 9/5) + 32;
        this.values = "Fahrenheit";
    } else {
        // Convert Fahrenheit to Celsius
        this.temperature = (this.temperature - 32) * 5/9;
        this.values = "Celsius";
    }
}

</script>

<style scoped>
.row {
  max-width: 1200px;
}

.container {
  background-color: #f5f5f5;
  width: 1200px;
  height: 900px;
}

figure {
  border: 2px solid black;
}
</style>