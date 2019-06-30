import React from 'react';
import {
  StyleSheet,
  View
} from 'react-native';
import { Button, Text } from 'native-base';
import firebase from '../shared/firebase';
import 'firebase/auth';

export default class BtnLogoutBtnLogout extends React.Component {
  static navigationOptions = {
    header: null,
  };

  onPressLogout = () => {
    firebase.auth().signOut();
  }

  render() {
    return (
      <View style={styles.wrapFooterStyle}>
        <Button full style={styles.btnStyle} onPress={() => this.onPressLogout()}>
          <Text style={styles.btnTextStyle}>Đăng xuất!</Text>
        </Button>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  wrapFooterStyle: {
    paddingTop: 20,
    paddingBottom: 20,
    width: '100%'
  },
  btnStyle: {
    marginLeft: 20,
    marginRight: 20,
    marginBottom: 20,
    height: 50,
    backgroundColor: '#ef5350'
  },
  btnTextStyle: {
    fontSize: 16,
    fontFamily: 'cereal-medium',
    color: 'white'
  }
});
