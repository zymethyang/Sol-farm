import React from 'react';
import {
  StyleSheet,
  View,
  Text,
  TouchableOpacity
} from 'react-native';

export default class NavComponent extends React.Component {
  render() {
    let { route } = this.props;
    return (
      <View style={styles.navStyle}>
        <TouchableOpacity style={styles.wrapTextNav} onPress={() => this.props.onPress('InfoScreen')}>
          <Text style={styles.textNavStyle}>Thông số</Text>
          <View style={{ width: '70%', height: 5, backgroundColor: route === 'info' ? '#00bfa5' : 'white', alignSelf: 'center', marginTop: 5 }}></View>
        </TouchableOpacity>
        <TouchableOpacity style={styles.wrapTextNav} onPress={() => this.props.onPress('AutoScreen')}>
          <Text style={styles.textNavStyle}>Tự động</Text>
          <View style={{ width: '70%', height: 5, backgroundColor: route === 'auto' ? '#00bfa5' : 'white', alignSelf: 'center', marginTop: 5 }}></View>
        </TouchableOpacity>
        <TouchableOpacity style={styles.wrapTextNav} onPress={() => this.props.onPress('ManualScreen')}>
          <Text style={styles.textNavStyle}>Thủ công</Text>
          <View style={{ width: '70%', height: 5, backgroundColor: route === 'manual' ? '#00bfa5' : 'white', alignSelf: 'center', marginTop: 5 }}></View>
        </TouchableOpacity>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  navStyle: {
    height: 60,
    flexDirection: 'row',
    borderTopColor: '#9e9e9e',
    borderTopWidth: 1,
    borderBottomColor: '#9e9e9e',
    borderBottomWidth: 1
  },
  wrapTextNav: {
    flex: 1,
    alignSelf: 'center',
    borderLeftWidth: 1,
    borderLeftColor: '#9e9e9e',
  },
  textNavStyle: {
    textAlign: 'center',
    fontSize: 18,
    fontFamily: 'cereal-medium',
    color: '#9e9e9e'
  }
});
