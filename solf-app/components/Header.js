import React from 'react';
import {
  StyleSheet,
  View,
  Text,
  Image
} from 'react-native';


export default class Header extends React.Component {
  render() {
    return (
      <View style={styles.container}>
        <View style={styles.imageStyle}>
          <Image source={require('../assets/images/hcmute.jpg')} style={styles.centerImage} />
        </View>
        <View style={styles.styleWrap}>
          <Text style={styles.centerText}>ĐỒ ÁN TỐT NGHIỆP</Text>
        </View>
        <View style={styles.imageStyle}>
          <Image source={require('../assets/images/ute.png')} style={styles.centerImage} />
        </View>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    paddingTop: Expo.Constants.statusBarHeight + 20,
    flexDirection: 'row',
    height: 120,
  },
  centerImage: {
    width: 60,
    height: 60,
    alignSelf: 'center'
  },
  imageStyle: {
    flex: 1
  },
  centerText: {
    textAlign: 'center',
    fontFamily: 'cereal-medium',
    fontSize: 20
  },
  styleWrap: {
    alignSelf: 'center',
    marginTop: -20
  }
});
