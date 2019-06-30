import React from 'react';
import {
  StyleSheet,
  Text,
  View,
  TextInput
} from 'react-native';

export default class SetupTextDouble extends React.Component {
  render() {
    let { title, value, type } = this.props;
    return (
      <View style={styles.container}>
        <View style={styles.flexOne}>
          <Text style={styles.alignCenter}>
            {title}
          </Text>
        </View>
        <View style={{ flex: 2, flexDirection: 'column' }}>
          <View style={{ flexDirection: 'row' }}>
            <TextInput
              style={styles.textBoxStyle}
              onChangeText={(text) => this.props.onChangeText({ type: `${type}on`, value: text })}
              value={value.on}
              placeholder="on"
            />
            <Text style={styles.property}>
              seconds
            </Text>
          </View>
          <View style={{ flexDirection: 'row' }}>
            <TextInput
              style={styles.textBoxStyle}
              onChangeText={(text) => this.props.onChangeText({ type: `${type}off`, value: text })}
              value={value.off}
              placeholder="off"
            />
            <Text style={styles.property}>
              seconds
            </Text>
          </View>
        </View>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    marginTop: 20,
    flexDirection: 'row',
    marginLeft: 20,
    marginRight: 20,
    paddingTop: 10,
    paddingBottom: 10,

    backgroundColor: 'white',
    shadowColor: '#000000',
    shadowOffset: {
      width: 0,
      height: 4
    },
    shadowRadius: 4.65,
    shadowOpacity: 0.30,
    elevation: 6
  },
  textBoxStyle: {
    height: 40,
    width: 50,
    textAlign: 'center'
  },
  flexOne: {
    flex: 3,
    justifyContent: 'flex-start'
  },
  arrowWrapStyle: {
    height: 40,
    justifyContent: 'center'
  },
  alignCenter: {
    alignSelf: 'flex-start',
    paddingLeft: 10,
    fontSize: 25,
    fontFamily: 'cereal-medium',
    color: '#9e9e9e'
  },
  property: {
    alignSelf: 'center',
    fontSize: 16,
    fontFamily: 'cereal-book',
    color: '#9e9e9e'
  }
});
