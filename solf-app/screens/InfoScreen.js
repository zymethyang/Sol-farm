import React from 'react';
import {
  StyleSheet,
  View,
} from 'react-native';

import { Message } from 'react-native-paho-mqtt';
import InfoBox from '../components/InfoScreen/InfoBox';
import SwitchUI from '../components/InfoScreen/SwitchUI';
import Footer from '../components/Footer';

import Amplify, { PubSub } from 'aws-amplify';
import { AWSIoTProvider } from '@aws-amplify/pubsub/lib/Providers';

Amplify.addPluggable(new AWSIoTProvider({
  aws_pubsub_region: 'ap-southeast-1',
  aws_pubsub_endpoint: 'wss://a2184o3gtkvd1o-ats.iot.ap-southeast-1.amazonaws.com/mqtt',
}));

Amplify.configure({
  Auth: {
    identityPoolId: 'ap-southeast-1:0c7bb479-7740-49e8-bad9-666bbc18d49c',
    region: 'ap-southeast-1',
  }
})

export default class InfoScreen extends React.Component {
  static navigationOptions = {
    header: null,
  };

  constructor(props) {
    super(props);
    this.state = {
      ph: 0,
      ec: 0,
      temp: 0,
      water: 0,
      spray: false,
      mixer: false
    }
  }

  async componentDidMount() {
    PubSub.subscribe('sol-farm/feedback').subscribe({
      next: data => console.log('Message received', data.value),
      error: error => console.error(error),
      close: () => console.log('Done'),
    });
    await PubSub.publish('sol-farm/control', { msg: 'Hello to all subscribers!' });
  }

  controlState = ({ type, value }) => {
    const state = { [type]: value };
    this.setState(state);
    const messageBuffer = JSON.stringify(state);
    const message = new Message(messageBuffer);
    message.destinationName = 'FWpfOR6wyKZIoYj';
    client.send(message);
  }

  render() {
    let { ph, ec, temp, water, spray, mixer } = this.state;
    return (
      <View style={styles.container}>
        <InfoBox title={"(1) Sht21 temp: "} value={ph} props="" />
        <InfoBox title={"(1) Sht21 hum: "} value={ec} props="mS/cm" />
        <InfoBox title={"(1) Ds1820b temp: "} value={temp} props="°C" />
        <InfoBox title={"(2) Sht21 temp: "} value={water} props="" />
        <InfoBox title={"(2) Sht21 hum: "} value={water} props="" />
        <InfoBox title={"(2) Fan: "} value={water} props="" />
        <View style={styles.rowsAlign}>
          <SwitchUI title={"Spray"} value={spray} onValueChange={value => this.controlState({ type: 'spray', value: value })} />
          <SwitchUI title={"Mixer"} value={mixer} onValueChange={value => this.controlState({ type: 'mixer', value: value })} />
        </View>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    paddingTop: Expo.Constants.statusBarHeight + 20,
    flex: 1,
    backgroundColor: '#fff',
  },
  rowsAlign: {
    flexDirection: 'row'
  }
});
