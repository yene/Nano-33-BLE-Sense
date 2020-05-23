<template>
  <div class="hello">
    <h1 class="title is-1"><strong>Make<span style="color:#086BA9">Zurich</span> Badge</strong></h1>
    <div class="notification is-warning" v-if="errorMessage !== ''">
      {{errorMessage}}
    </div>

    <div v-if="server === null">
      <button class="button" v-on:click="startScan" style="margin-left: 10px;">Connect to Badge with BLE</button>
    </div>
    <div v-else>
      <div>
        <h1 class="title is-3">Connected to {{name}}</h1>
        <h2 class="subtitle">Interact with the Badge</h2>

        <div class="led-ring">
          <input type="color" value="#000" style="top: 381px;left: 363px;" name="led-10" v-on:input="colorWheelUpdate(10, $event)">
          <input type="color" value="#000" style="top: 297px;left: 450px;" name="led-9" v-on:input="colorWheelUpdate(9, $event)">
          <input type="color" value="#000" style="top: 193px;left: 467px;" name="led-8" v-on:input="colorWheelUpdate(8, $event)">
          <input type="color" value="#000" style="top: 103px;left: 441px;" name="led-7" v-on:input="colorWheelUpdate(7, $event)">
          <input type="color" value="#000" style="top: 22px;left: 325px" name="led-6" v-on:input="colorWheelUpdate(6, $event)">
          <input type="color" value="#000" style="top: 22px;left: 132px;" name="led-5" v-on:input="colorWheelUpdate(5, $event)">
          <input type="color" value="#000" style="top: 103px;left: 33px;" name="led-4" v-on:input="colorWheelUpdate(4, $event)">
          <input type="color" value="#000" style="top: 193px;left: 5px;" name="led-3" v-on:input="colorWheelUpdate(3, $event)">
          <input type="color" value="#000" style="top: 297px;left: 30px;" name="led-2" v-on:input="colorWheelUpdate(2, $event)">
          <input type="color" value="#000" style="top: 381px;left: 116px;" name="led-1" v-on:input="colorWheelUpdate(1, $event)">
          <input type="color" value="#000" style="top: 406px;left: 242px;" name="led-0" v-on:input="colorWheelUpdate(0, $event)">
        </div>

        <div class="">
          <div class="select">
            <select v-model="selectedCommand" class="select">
              <option value="0" selected>Stop and clear colors</option>
              <option value="1" selected>Rainbow Wheel</option>
            </select>
          </div>
          <button class="button" v-on:click="sendCommand">Send Command</button>
        </div>

        <table class="table">
          <thead>
            <tr>
              <th><abbr title="Characteristic UUID">Characteristic UUID</abbr></th>
              <th>Feature</th>
              <th><abbr title="Based on the UUID we can make the values human readable.">Value</abbr></th>
              <th><abbr title="Some characteristics allow to be read, written or notified on new values.">Action</abbr></th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td>0D0F17BC-1528-48C0-B303-2267D900CF25</td>
              <td>Device Address (Serial)</td>
              <td>{{deviceAddr}}</td>
              <td><button class="button" v-on:click="readDeviceAddr">Read</button></td>
            </tr>
            <tr>
              <td>2A6E</td>
              <td>Temperature</td>
              <td>{{temperature}}</td>
              <td><button class="button" v-on:click="readTemperature">Read</button></td>
            </tr>
            <tr>
              <td>2A6F</td>
              <td>Humidity</td>
              <td>{{humidity}}</td>
              <td><button class="button" v-on:click="readHumidity">Read</button></td>
            </tr>
            <tr>
              <td>2A6D</td>
              <td>Pressure</td>
              <td>{{pressure}}</td>
              <td><button class="button" v-on:click="readPressure">Read</button></td>
            </tr>

            <tr>
              <td>8E4B75FE-8148-4775-8D48-71B51DF62C46</td>
              <td>Accelerometer</td>
              <td>{{accelerometer}}</td>
              <td><button class="button" v-on:click="readAccelerometer">Read</button></td>
            </tr>
            <tr>
              <td>8E4B75FE-8148-4775-8D48-71B51DF62C47</td>
              <td>Gyroscope</td>
              <td>{{gyroscope}}</td>
              <td><button class="button" v-on:click="readGyroscope">Read</button></td>
            </tr>
            <tr>
              <td>8E4B75FE-8148-4775-8D48-71B51DF62C48</td>
              <td>Magnetometer</td>
              <td>{{magnetometer}}</td>
              <td><button class="button" v-on:click="readMagnetometer">Read</button></td>
            </tr>

            <tr>
              <td>1648A1AE-5A8D-4544-B3CF-6C497D73E771</td>
              <td>Proximitry</td>
              <td>{{proximitry}}</td>
              <td><button class="button" v-on:click="readProximitry">Read</button></td>
            </tr>
            <tr>
              <td>1648A1AE-5A8D-4544-B3CF-6C497D73E772</td>
              <td>Detected Color</td>
              <td>{{detectedColor}}</td>
              <td><button class="button" v-on:click="readDetectedColor">Read</button></td>
            </tr>
            <tr>
              <td>1648A1AE-5A8D-4544-B3CF-6C497D73E773</td>
              <td>Light Intensity</td>
              <td>{{lightIntensity}}</td>
              <td><button class="button" v-on:click="readLightIntensity">Read</button></td>
            </tr>
            <tr>
              <td>1648A1AE-5A8D-4544-B3CF-6C497D73E774</td>
              <td>Gesture</td>
              <td>{{gesture}}</td>
              <td><button class="button" v-on:click="readGesture">Read</button></td>
            </tr>

            <tr>
              <td>3D05109A-7DE0-4146-9EE8-9283AC9E4B14</td>
              <td>System RGB</td>
              <td>
                <select v-model="rgbInput">
                  <option value="0" selected >OFF</option>
                  <option value="1" selected >RED</option>
                  <option value="2" selected >GREEN</option>
                  <option value="3" selected >BLUE</option>
                </select>
              <td><button class="button" v-on:click="writeRGB">Write</button></td>
            </tr>
            <tr>
              <td>3D05109A-7DE0-4146-9EE8-9283AC9E4B12</td>
              <td>System LED</td>
              <td><input type="checkbox" v-model="ledInput"></td>
              <td><button class="button" v-on:click="writeLED">Write</button></td>
            </tr>

          </tbody>
        </table>
      </div>
      <button class="button" v-on:click="disconnect()">Disconnect</button>
    </div>

    <a href="https://store.arduino.cc/arduino-nano-33-ble-sense-with-headers" target="_blank">More information about the Arduino NANO 33 BLE Sense</a>

  </div>
</template>

<script>

import ColorPicker from './ColorPicker.vue';

let servicesList = [0x181A, 'a4e649f4-4be5-11e5-885d-feff819cdc9f', 'e2f101c1-8fa8-4bcb-80fc-6469f25acdb6', '3d05109a-7de0-4146-9ee8-9283ac9e4b00', 'ea0669f3-fd44-406f-98e0-5feaeed5cb22'];
let decoder = new TextDecoder();
let encoder = new TextEncoder();

export default {
  name: 'BleScanner',
  components: {ColorPicker},
  data() {
    return {
      server: null,
      name: '',
      errorMessage: '',
      characteristics: new Map(),
      deviceAddr: '--',
      temperature: '-- °C',
      humidity: '-- %',
      pressure: '-- kPa',
      accelerometer: '',
      gyroscope: '',
      magnetometer: '',
      proximitry: '--',
      detectedColor: '--',
      lightIntensity: '--',
      gesture: '--',
      rgbInput: '0',
      ledInput: false,
      selectedCommand: 0,
    }
  },
  computed: {

  },
  methods: {
    sendCommand() {
      let char = this.characteristics.get('3d05109a-7de0-4146-9ee8-9283ac9e4b16');
      if (char === undefined) {
        this.errorMessage = 'Command characteristics does not exist';
        return;
      }
      let arr = Uint8Array.of(Number(this.selectedCommand));
      char.writeValue(arr).then(() => {
        console.log('value has been sent', arr);
      })
      .catch(error => {
        this.errorMessage = 'Error when trying to write Command, ' + error;
      });
    },
    colorWheelUpdate(ledPosition, event) {
      let newcolor = event.target.value;
      console.log(hexToRGB(newcolor));
      clearTimeout(this._colorDebounce);
      this._colorDebounce = setTimeout(() => {
        this.setRingLED(ledPosition, hexToRGB(newcolor));
      }, 500);
    },
    readDeviceAddr() {
      let char = this.characteristics.get('0d0f17bc-1528-48c0-b303-2267d900cf25');
      if (char === undefined) {
        this.errorMessage = 'Device Address characteristics does not exist';
        return;
      }
      char.readValue().then((value) => {
        this.deviceAddr = buf2hex(value.buffer).toUpperCase();
      })
      .catch(error => {
        this.errorMessage = 'Error when trying to read Device Address, ' + error;
      });
    },
    writeRGB() {
      let char = this.characteristics.get('3d05109a-7de0-4146-9ee8-9283ac9e4b14');
      if (char === undefined) {
        this.errorMessage = 'System RGB characteristics does not exist';
        return;
      }
      let arr = Uint8Array.of(Number(this.rgbInput));
      char.writeValue(arr).then(() => {
        console.log('value has been sent', arr);
      })
      .catch(error => {
        this.errorMessage = 'Error when trying to write System RGB, ' + error;
      });
    },
    writeLED() {
      let char = this.characteristics.get('3d05109a-7de0-4146-9ee8-9283ac9e4b12');
      if (char === undefined) {
        this.errorMessage = 'System LED characteristics does not exist';
        return;
      }
      let v = Boolean(this.ledInput);
      let arr = Uint8Array.of(v);
      char.writeValue(arr).then(() => {
        console.log('value has been sent', arr);
      })
      .catch(error => {
        this.errorMessage = 'Error when trying to write System LED, ' + error;
      });
    },
    readTemperature() {
      let char = this.characteristics.get('00002a6e-0000-1000-8000-00805f9b34fb');
      if (char === undefined) {
        this.errorMessage = 'Temperature characteristics does not exist';
        return;
      }
      char.readValue().then((value) => {
        this.temperature = (value.getInt16(0, true) / 100) + ' °C';
      })
      .catch(error => {
        this.errorMessage = 'Error when trying to read temperature, ' + error;
      });
    },
    readHumidity() {
      let char = this.characteristics.get('00002a6f-0000-1000-8000-00805f9b34fb');
      if (char === undefined) {
        this.errorMessage = 'Humidity characteristics does not exist';
        return;
      }
      char.readValue().then((value) => {
        this.humidity = (value.getInt16(0, true) / 100) + ' %';
      })
      .catch(error => {
        this.errorMessage = 'Error when trying to read Humidity, ' + error;
      });
    },
    readPressure() {
      let char = this.characteristics.get('00002a6d-0000-1000-8000-00805f9b34fb');
      if (char === undefined) {
        this.errorMessage = 'Humidity characteristics does not exist';
        return;
      }
      char.readValue().then((value) => {
        let v = value.getFloat32(0, true);
        v = Math.round(v * 100) / 100
        this.pressure = v + ' kPa';
      })
      .catch(error => {
        this.errorMessage = 'Error when trying to read Humidity, ' + error;
      });
    },
    readAccelerometer() {
      let char = this.characteristics.get('8e4b75fe-8148-4775-8d48-71b51df62c46');
      if (char === undefined) {
        this.errorMessage = 'Accelerometer characteristics does not exist';
        return;
      }
      char.readValue().then((value) => {
        let v = decoder.decode(value);
        this.accelerometer = v;
      })
      .catch(error => {
        this.errorMessage = 'Error when trying to read Accelerometer, ' + error;
      });
    },
    readGyroscope() {
      let char = this.characteristics.get('8e4b75fe-8148-4775-8d48-71b51df62c47');
      if (char === undefined) {
        this.errorMessage = 'Gyroscope characteristics does not exist';
        return;
      }
      char.readValue().then((value) => {
        let v = decoder.decode(value);
        this.gyroscope = v;
      })
      .catch(error => {
        this.errorMessage = 'Error when trying to read Gyroscope, ' + error;
      });
    },
    readMagnetometer() {
      let char = this.characteristics.get('8e4b75fe-8148-4775-8d48-71b51df62c48');
      if (char === undefined) {
        this.errorMessage = 'Magnetometer characteristics does not exist';
        return;
      }
      char.readValue().then((value) => {
        let v = decoder.decode(value);
        this.magnetometer = v;
      })
      .catch(error => {
        this.errorMessage = 'Error when trying to read Magnetometer, ' + error;
      });
    },
    readProximitry() {
      let char = this.characteristics.get('1648a1ae-5a8d-4544-b3cf-6c497d73e771');
      if (char === undefined) {
        this.errorMessage = 'Proximitry characteristics does not exist';
        return;
      }
      char.readValue().then((value) => {
        this.proximitry = value.getUint8(0, true);
      })
      .catch(error => {
        this.errorMessage = 'Error when trying to read Proximitry, ' + error;
      });
    },
    readDetectedColor() {
      let char = this.characteristics.get('1648a1ae-5a8d-4544-b3cf-6c497d73e772');
      if (char === undefined) {
        this.errorMessage = 'Detected Color characteristics does not exist';
        return;
      }
      char.readValue().then((value) => {
        let v = decoder.decode(value);
        this.detectedColor = v;
      })
      .catch(error => {
        this.errorMessage = 'Error when trying to read Detected Color, ' + error;
      });
    },
    readLightIntensity() {
      let char = this.characteristics.get('1648a1ae-5a8d-4544-b3cf-6c497d73e773');
      if (char === undefined) {
        this.errorMessage = 'Light Intentsity characteristics does not exist';
        return;
      }
      char.readValue().then((value) => {
        this.lightIntensity = value.getUint8(0, true);
      })
      .catch(error => {
        this.errorMessage = 'Error when trying to read Light Intentsity, ' + error;
      });
    },
    readGesture() {
      let char = this.characteristics.get('1648a1ae-5a8d-4544-b3cf-6c497d73e774');
      if (char === undefined) {
        this.errorMessage = 'Gesture characteristics does not exist';
        return;
      }
      char.readValue().then((value) => {
        let v = value.getInt8(0);
        this.gesture = gestureDirection(v);
      })
      .catch(error => {
        this.errorMessage = 'Error when trying to read Gesture, ' + error;
      });

    },
    setRingLED(pos, rgbArray) {
      const ledRing = '3d05109a-7de0-4146-9ee8-9283ac9e4b15';
      let char = this.characteristics.get(ledRing);
      if (char === undefined) {
        this.errorMessage = 'Ring LED characteristics does not exist';
        return;
      }
      let arr = new Uint8Array([pos, rgbArray[0], rgbArray[1], rgbArray[2]]); // led number, red, green, blue in decimal
      char.writeValue(arr).then(() => {
        console.log('value has been sent', arr);
      })
      .catch(error => {
        this.errorMessage = 'Error when trying to write Ring LED, ' + error;
      });
    },

    startScan(e) {
      var options = {
        filters: [
          {services: ['ea0669f3-fd44-406f-98e0-5feaeed5cb22']},
        ],
        optionalServices: servicesList
      };
      navigator.bluetooth.requestDevice(options)
      .then((device) => {
        this.name = device.name;
        console.log(device);
        device.addEventListener('gattserverdisconnected', this.onDisconnected);
        return device.gatt.connect();
      })
      .then((server) => {
        this.server = server;
        return server.getPrimaryServices(); // gets all services listed in filters.services and optionalServices
      })
      .then((services) => {
        console.log('Getting Characteristics...');
        let queue = Promise.resolve();
        services.forEach(service => {
          queue = queue.then(_ => service.getCharacteristics().then(characteristics => {
            console.log('> Service: ' + service.uuid);
            characteristics.forEach(characteristic => {
              console.log('>> Characteristic: ' + characteristic.uuid + ' ' + getSupportedProperties(characteristic));
              this.characteristics.set(characteristic.uuid, characteristic);
            });
          }));
        });
        return queue;
      })
      .catch((error) => {
        console.log(error);
        this.errorMessage = error;
      });
    },
    disconnect() {
      this.server.disconnect();
    },
    onDisconnected(e) {
      let device = e.target;
      this.errorMessage = 'Device ' + device.name + ' is disconnected.';
      this.server = null;
    },
    formatHex(n) {
      return '0x' + n.toString(16).toUpperCase();
    }
  }
}

/* Utils */

function getSupportedProperties(characteristic) {
  let supportedProperties = [];
  for (const p in characteristic.properties) {
    if (characteristic.properties[p] === true) {
      supportedProperties.push(p.toUpperCase());
    }
  }
  return '[' + supportedProperties.join(', ') + ']';
}

function buf2hex(buffer) { // buffer is an ArrayBuffer
  return Array.prototype.map.call(new Uint8Array(buffer), x => ('00' + x.toString(16)).slice(-2)).join('');
}

function gestureDirection(number) {
  switch(number) {
    case -1:
      return 'NONE';
      break;
    case 0:
      return 'UP';
      break;
    case 1:
      return 'DOWN';
      break;
    case 2:
      return 'LEFT';
      break;
    case 3:
      return 'RIGHT';
      break;
    default:
      // code block
  }
}

function hexToRGB(h) {
  let r = 0, g = 0, b = 0;
  if (h.length == 4) { // 3 digits
    r = "0x" + h[1] + h[1];
    g = "0x" + h[2] + h[2];
    b = "0x" + h[3] + h[3];
  } else if (h.length == 7) { // 6 digits
    r = "0x" + h[1] + h[2];
    g = "0x" + h[3] + h[4];
    b = "0x" + h[5] + h[6];
  }
  return [parseInt(r, 16), parseInt(g, 16), parseInt(b, 16)];
}


</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
.led-ring {
  position: relative;
  background-image: url('/led-ring.jpg');
  background-size: 500px;
  width: 500px;
  height: 446px;
}

.led-ring input {
  position: absolute;
}

a {
  color: #42b983;
}
</style>
