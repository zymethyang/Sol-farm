import React from 'react';
import {
  StyleSheet,
  View,
  AsyncStorage
} from 'react-native';
import Footer from '../components/Footer';
import ImageSection from '../components/AutoScreen/ImageSection';
import InfoSection from '../components/AutoScreen/InfoSection';
import ButtonSection from '../components/AutoScreen/ButtonSection';
import { client } from '../shared/mqtt';
import { Message } from 'react-native-paho-mqtt';

export default class AutoScreen extends React.Component {
  static navigationOptions = {
    header: null,
  };

  constructor(props) {
    super(props);
    this.state = {
      params: {
        name: '',
        phMin: 0,
        phMax: 0,
        ecMin: 0,
        ecMax: 0,
        image: require('../assets/images/vegetables/white.jpg')
      }
    }
  }

  componentWillReceiveProps() {
    setTimeout(() => {
      let { params } = this.props.navigation.state;
      if (params) {
        this.setState({ params: params });
        const paramsBuffer = JSON.stringify(params);
        AsyncStorage.setItem('params', paramsBuffer);

        this.sendCloud({
          ph_min: params.phMin,
          ph_max: params.phMax,
          ec_min: params.ecMin,
          ec_max: params.ecMax,
          on: 180,
          off: 60
        });
      }
    })
  }

  sendCloud = (data) => {
    const dataBuffer = JSON.stringify(data);
    const message = new Message(dataBuffer);
    message.destinationName = 'FWpfOR6wyKZIoYj';
    client.send(message);
  }

  componentDidMount() {
    AsyncStorage.getItem('params').then((paramsBuffer) => {
      if (paramsBuffer) {
        const params = JSON.parse(paramsBuffer);
        this.setState({ params: params })
      }
    })
  }

  processButton = (type) => {
    switch (type) {
      case 'change_tree':
        const { navigate } = this.props.navigation;
        navigate('ListTrees');
        break;
      case 'stop_system':
        this.sendCloud({ status: false })
        break;
    }
  }

  render() {

    const { name, phMin, phMax, ecMin, ecMax, image } = this.state.params;
    return (
      <View style={styles.container}>
        <View style={styles.wrapShadow}>
          <View style={{ flexDirection: 'row' }}>
            <ImageSection source={image} />
            <InfoSection name={name} ph={` ${phMin} → ${phMax}`} ec={` ${ecMin} → ${ecMax}`} />
          </View>
          <ButtonSection onPress={({ type }) => this.processButton(type)} />
        </View>
        <Footer />
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    paddingTop: Expo.Constants.statusBarHeight + 50,
    flex: 1,
    backgroundColor: '#fff',
  },
  wrapShadow: {
    backgroundColor: 'white',
    shadowColor: '#000000',
    shadowOffset: {
      width: 0,
      height: 1
    },
    shadowRadius: 2.22,
    shadowOpacity: 0.22,
    elevation: 3,

    marginLeft: 10,
    marginRight: 10,
    paddingBottom: 20
  }
});
