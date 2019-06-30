import React from 'react';
import {
  StyleSheet,
  ScrollView
} from 'react-native';


import SetupText from '../components/ManualScreen/SetupText';
import SetupTextDouble from '../components/ManualScreen/SetupTextDouble';
import BtnSave from '../components/ManualScreen/BtnSave';


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


export default class ManualScreen extends React.Component {
  static navigationOptions = {
    header: null,
  };

  constructor(props) {
    super(props);
    this.state = {
      k1on: "",
      k1off: "",
      k2on: "",
      k2off: "",
      k3on: "",
      k3off: ""
    }
  }

  sendCloud = ({ k1on, k1off, k2on, k2off, k3on, k3off }) => {
    const dataBuffer = {
      nodeID: 'esp32',
      k1on: parseInt(k1on),
      k1off: parseInt(k1off),
      k2on: parseInt(k2on),
      k2off: parseInt(k2off),
      k3on: parseInt(k3on),
      k3off: parseInt(k3off)
    };
    PubSub.publish('sol-farm/control', dataBuffer);
  }

  render() {
    let { k1on, k1off, k2on, k2off, k3on, k3off } = this.state;
    return (
      <ScrollView style={styles.container}>
        <SetupText title="(2) fan" type="2" value={100} onSlidingComplete={({ event }) => PubSub.publish('sol-farm/control', { dacValue: parseInt(event), nodeID: 'node02' })} />
        <SetupText title="(e) fan" type="e" value={200} onSlidingComplete={({ event }) => PubSub.publish('sol-farm/control', { esc: parseInt(event), nodeID: 'esp32' })} />
        <SetupTextDouble title="K1" type="k1" value={{ on: k1on, off: k1off }} onChangeText={({ type, value }) => this.setState({ [type]: value })} />
        <SetupTextDouble title="K2" type="k2" value={{ on: k2on, off: k2off }} onChangeText={({ type, value }) => this.setState({ [type]: value })} />
        <SetupTextDouble title="K3" type="k3" value={{ on: k3on, off: k3off }} onChangeText={({ type, value }) => this.setState({ [type]: value })} />
        {/* 
        <SetupSwitch value={spray_outside} onValueChange={({ type, value }) => this.setState({ [type]: value })} />
        <View style={styles.wrapStyle}>
          <Text style={styles.textStyle}>Do you want to run?</Text>
        </View>
        */}
        <BtnSave onPress={() => this.sendCloud(this.state)} />
      </ScrollView>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    paddingTop: Expo.Constants.statusBarHeight + 20,
    flex: 1,
    backgroundColor: '#fff',
  },
  wrapStyle: {
    position: 'absolute',
    bottom: 100,
    left: 0,
    width: '100%'
  },
  textStyle: {
    fontSize: 18,
    fontFamily: 'cereal-medium',
    color: '#9e9e9e',
    textAlign: 'center'
  }
});
