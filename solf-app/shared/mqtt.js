import { Client } from 'react-native-paho-mqtt';
require('events').EventEmitter.prototype._maxListeners = 1000;

const myStorage = {
    setItem: (key, item) => {
        myStorage[key] = item;
    },
    getItem: (key) => myStorage[key],
    removeItem: (key) => {
        delete myStorage[key];
    },
};

export const client = new Client({ uri: 'wss://mqtt.solavo.net:8883/', clientId: "android_" + parseInt(Math.random() * 100, 10), storage: myStorage });
var options = {
    useSSL: false,
    userName: "FWpfOR6wyKZIoYj",
    password: "135308641",
}

export const mqtt = client.connect(options);