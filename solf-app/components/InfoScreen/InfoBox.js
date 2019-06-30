import React from 'react';
import {
  StyleSheet,
  Text,
  View,
} from 'react-native';

export default class InfoBox extends React.Component {
  render() {
    let { title, value, props } = this.props;
    return (
      <View style={styles.container}>
        <View style={styles.wrapText}>
          <Text style={styles.textStyle}>{title}</Text>
        </View>
        <View style={styles.wrapNumber}>
          <Text style={styles.textNumber}>{value} {props}</Text>
        </View>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    height: 70,
    flexDirection: 'row',
    paddingLeft: 30,
  },
  wrapText: {
    alignSelf: 'center',
  },
  wrapNumber: {
    alignSelf: 'center',
    marginLeft: 10,
  },
  textStyle: {
    fontSize: 25,
    fontFamily: 'cereal-medium',
    color: '#9e9e9e'
  },
  textNumber: {
    fontSize: 25,
    fontFamily: 'cereal-medium',
    color: '#00bfa5'
  }
});
