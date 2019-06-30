import React from 'react';
import {
  StyleSheet,
  Text,
  View,
  TextInput,
  Slider
} from 'react-native';

export default class SetupText extends React.Component {
  render() {
    let { title, type, value } = this.props;
    return (
      <View style={styles.container}>
        <View style={styles.flexOne}>
          <Text style={styles.alignCenter}>
            {title}
          </Text>
        </View>
        <View style={{ flex: 2, flexDirection: 'row' }}>
          <Slider onSlidingComplete={event=>this.props.onSlidingComplete({event:event,type:type})} value={value} style={{ width: 200, height: 40 }} minimumTrackTintColor="#ef5350" maximumTrackTintColor="#9e9e9e" minimumValue={0} maximumValue={1000} thumbTintColor="transparent" />
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
    width: 80,
    textAlign: 'center'
  },
  flexOne: {
    flex: 1,
    justifyContent: 'center'
  },
  arrowWrapStyle: {
    height: 40,
    justifyContent: 'center'
  },
  alignCenter: {
    alignSelf: 'center',
    paddingRight: 10,
    fontSize: 25,
    fontFamily: 'cereal-medium',
    color: '#9e9e9e'
  }
});
