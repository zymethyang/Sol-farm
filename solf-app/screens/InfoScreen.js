import React from 'react';
import {
  StyleSheet,
  View,
} from 'react-native';
import InfoBox from '../components/InfoScreen/InfoBox';
import { PubSub } from 'aws-amplify';

class InfoScreen extends React.Component {
  static navigationOptions = {
    header: null,
  };

  constructor(props) {
    super(props);
    this.state = {
      node01: {
        tempSHT21: 0,
        humSHT21: 0,
        ds18b20: 0,
        nodeID: "node01"
      },
      node02: {
        tempSHT21: 0,
        humSHT21: 0,
        dacValue: 0,
        nodeID: "node02"
      },
      esp32: {
        nodeID: "esp32",
        pressure: false,
        esc: 0,
        relayOne: false,
        relayTwo: false,
        relayThree: false
      }
    }
  }

  processingFeedbackValue = (data) => {
    console.log(data);
    switch (data.nodeID) {
      case 'node01':
        this.setState({
          node01: data
        })
        break;
      case 'node02':
        this.setState({
          node02: data
        })
        break;
      case 'esp32':
        this.setState({
          esp32: data
        })
        break;
    }
  }

  async componentDidMount() {
    PubSub.subscribe('sol-farm/feedback', { provider: 'AWSIoTProvider' }).subscribe({
      next: data => this.processingFeedbackValue(data.value),
      error: error => console.error(error),
      close: () => console.log('Done'),
    });
  }


  render() {
    let { node01, node02, esp32 } = this.state;
    return (
      <View style={styles.container}>
        <InfoBox title={"(1) Sht21 temp: "} value={node01.tempSHT21} props="°C" />
        <InfoBox title={"(1) Sht21 hum: "} value={node01.humSHT21} props="%" />
        <InfoBox title={"(1) Ds1820b temp: "} value={node01.ds18b20} props="°C" />
        <InfoBox title={"(2) Sht21 temp: "} value={node02.tempSHT21} props="°C" />
        <InfoBox title={"(2) Sht21 hum: "} value={node02.humSHT21} props="%" />
        <InfoBox title={"(2) Fan: "} value={node02.dacValue} props="" />
        <InfoBox title={"(ESP) Fan: "} value={esp32.esc} props="" />
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

export default InfoScreen;