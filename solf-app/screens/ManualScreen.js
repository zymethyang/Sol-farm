import React from 'react';
import {
  StyleSheet,
  ScrollView
} from 'react-native';
import { View } from 'native-base';

import SetupText from '../components/ManualScreen/SetupText';
import SetupTextDouble from '../components/ManualScreen/SetupTextDouble';
import Button from '../components/ManualScreen/Button';

import { PubSub } from 'aws-amplify';

class ManualScreen extends React.Component {
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

  sendCircleCloud = ({ k1on, k1off, k2on, k2off, k3on, k3off }) => {
    const dataBuffer = {
      nodeID: 'esp32',
      k1on: parseInt(k1on) || 0,
      k1off: parseInt(k1off) || 0,
      k2on: parseInt(k2on) || 0,
      k2off: parseInt(k2off) || 0,
      k3on: parseInt(k3on) || 0,
      k3off: parseInt(k3off) || 0
    };
    PubSub.publish('sol-farm/control', dataBuffer);
  }

  stopFan2 = () => {
    const dataBuffer = {
      nodeID: 'node02',
      dacValue: 0
    };
    PubSub.publish('sol-farm/control', dataBuffer);
  }

  stopFanE = () => {
    const dataBuffer = {
      nodeID: 'esp32',
      esc: 0
    };
    PubSub.publish('sol-farm/control', dataBuffer);
  }

  render() {
    let { k1on, k1off, k2on, k2off, k3on, k3off } = this.state;
    return (
      <ScrollView style={styles.container}>
        <SetupText title="(2) fan" type="2" value={100} onSlidingComplete={({ event }) => PubSub.publish('sol-farm/control', { dacValue: parseInt(event), nodeID: 'node02' })} />
        <SetupText title="(e) fan" type="e" value={200} onSlidingComplete={({ event }) => PubSub.publish('sol-farm/control', { esc: parseInt(event), nodeID: 'esp32' })} />
        <Button title="Stop (2) fan" onPress={() => this.stopFan2()} />
        <Button title="Stop (e) fan" onPress={() => this.stopFanE()} />
        <SetupTextDouble title="K1" type="k1" value={{ on: k1on, off: k1off }} onChangeText={({ type, value }) => this.setState({ [type]: value })} />
        <SetupTextDouble title="K2" type="k2" value={{ on: k2on, off: k2off }} onChangeText={({ type, value }) => this.setState({ [type]: value })} />
        <SetupTextDouble title="K3" type="k3" value={{ on: k3on, off: k3off }} onChangeText={({ type, value }) => this.setState({ [type]: value })} />
        <View style={{ marginBottom: 80 }}>
          <Button title="Save" onPress={() => this.sendCircleCloud(this.state)} />
        </View>
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

export default ManualScreen;