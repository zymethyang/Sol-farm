import React from 'react';
import {
  StyleSheet,
  View,
  TextInput,
  Alert
} from 'react-native';

import { Button, Text } from 'native-base';
import firebase from '../shared/firebase';
import 'firebase/auth';

export default class LoginScreen extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      email: '',
      password: ''
    }
  }

  onPressLogin = (email, password) => {
    firebase.auth().signInWithEmailAndPassword(email, password).catch((error) => {
      var errorCode = error.code;
      var errorMessage = error.message;
      console.log(error);
      if (errorCode) {
        Alert.alert("Thông báo", "Đăng nhập thất bại!");
      }
    });
  }

  render() {
    let { email, password } = this.state;
    return (
      <View style={styles.container}>
        <View style={styles.wrapUsername}>
          <TextInput
            style={styles.placeholderStyle}
            onChangeText={(email) => this.setState({ email })}
            value={email}
            placeholder='Email'
          />
        </View>
        <View style={styles.wrapPassword}>
          <TextInput
            style={styles.placeholderStyle}
            onChangeText={(password) => this.setState({ password })}
            value={password}
            secureTextEntry={true}
            placeholder='Mật khẩu'
          />
        </View>
        <Button full style={styles.wrapLoginBtn} onPress={() => this.onPressLogin(email, password)}>
          <Text style={styles.loginText}>ĐĂNG NHẬP</Text>
        </Button>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    paddingTop: Expo.Constants.statusBarHeight,
    flex: 1,
    flexDirection: 'column',
    backgroundColor: '#fff',
    alignContent: 'center'
  },
  loadingContainer: {
    flex: 1,
    justifyContent: 'center'
  },
  loadingStyle: {
    textAlign: 'center',
    color: '#00bfa5'
  },
  wrapPassword: {
    marginTop: '15%',
    width: '80%',
    alignSelf: 'center',
  },
  wrapLoginBtn: {
    marginTop: 40,
    width: '80%',
    height: 60,
    alignSelf: 'center',
    backgroundColor: '#00bfa5',
    borderRadius: 35
  },
  wrapUsername: {
    marginTop: '50%',
    width: '80%',
    alignSelf: 'center',
  },
  loginText: {
    fontFamily: 'cereal-medium',
    fontSize: 20
  },
  placeholderStyle: {
    height: 60,
    borderColor: '#00bfa5',
    borderWidth: 3,
    borderRadius: 35,
    textAlign: 'center',
    fontFamily: 'cereal-medium',
    fontSize: 20
  }
});
