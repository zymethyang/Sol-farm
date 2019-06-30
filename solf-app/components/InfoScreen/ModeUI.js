import React from 'react';
import {
  StyleSheet,
  Text,
  View,
} from 'react-native';

export default class ModeUI extends React.Component {
  render() {
    let { title, value } = this.props;
    return (
      <View style={styles.container}>
        <View style={styles.wrapText}>
          <Text style={styles.textStyle}>{title}</Text>
        </View>
        <View style={styles.wrapNumber}>
          <Text style={styles.textStyle}>{value === 1 ? ': Tự động' : value === 2 ? ': Thủ công' : ': Chưa xác định'}</Text>
        </View>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    height: 80,
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
