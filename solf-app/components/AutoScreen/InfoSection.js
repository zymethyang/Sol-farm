import React from 'react';
import {
  StyleSheet,
  Text,
  View,
  Image
} from 'react-native';


export default class InfoSection extends React.Component {
  render() {
    let { name, ph, ec } = this.props;
    return (
      <View style={styles.container}>
        <View style={{ paddingBottom: 15 }}>
          <Text style={styles.nameStyle}>{name}</Text>
        </View>
        <View style={styles.wrapText}>
          <Text style={styles.fieldStyle}>pH:</Text>
          <Text style={styles.valueStyle}>{ph}</Text>
        </View>
        <View style={styles.wrapText}>
          <Text style={styles.fieldStyle}>EC:</Text>
          <Text style={styles.valueStyle}>{ec}</Text>
        </View>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    paddingTop: 20
  },
  nameStyle: {
    fontSize: 25,
    fontFamily: 'cereal-medium',
    color: '#00bfa5'
  },
  fieldStyle: {
    fontSize: 25,
    fontFamily: 'cereal-medium',
    color: '#9e9e9e'
  },
  valueStyle: {
    fontSize: 25,
    fontFamily: 'cereal-medium',
    color: '#00bfa5'
  },
  wrapText: {
    flexDirection: 'row',
    paddingBottom: 15
  }
});
