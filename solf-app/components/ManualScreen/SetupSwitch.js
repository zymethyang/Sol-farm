import React from 'react';
import {
  StyleSheet,
  Text,
  View
} from 'react-native';
import { Switch } from 'native-base';

export default class SetupText extends React.Component {
  render() {
    let { value } = this.props;
    return (
      <View style={styles.container}>
        <View style={styles.flexOne}>
          <Text style={styles.alignCenter}>
            Spray outside
          </Text>
        </View>
        <View style={{ flex: 1, alignSelf: 'center' }}>
          <Switch value={value} onValueChange={(value) => this.props.onValueChange({ type: 'spray_outside', value: value })} />
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
    paddingTop: 20,
    paddingBottom: 20,

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
  flexOne: {
    flex: 2,
    justifyContent: 'flex-start'
  },
  alignCenter: {
    paddingLeft: 10,
    paddingRight: 10,
    fontSize: 25,
    fontFamily: 'cereal-medium',
    color: '#9e9e9e'
  }
});
