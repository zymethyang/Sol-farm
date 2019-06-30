import React from 'react';
import {
  StyleSheet,
  Text,
  View,
} from 'react-native';
import { Switch } from 'native-base';

export default class SwitchUI extends React.Component {
  render() {
    let { title, value } = this.props;
    return (
      <View style={styles.container}>
        <View style={styles.wrapText}>
          <Text style={styles.textStyle}>{title}</Text>
        </View>
        <View style={styles.wrapNumber}>
          <Switch value={value} onValueChange={(value) => this.props.onValueChange(value)} />
        </View>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    height: 100,
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
  }
});
