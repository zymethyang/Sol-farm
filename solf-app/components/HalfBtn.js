import React from 'react';
import {
  StyleSheet,
  View
} from 'react-native';
import { Button, Text } from 'native-base';


export default class HalfBtn extends React.Component {
  static navigationOptions = {
    header: null,
  };

  render() {
    let { title } = this.props;
    return (
      <Button full style={styles.btnStyle} onPress={() => this.props.onPress()}>
        <Text style={styles.btnTextStyle}>{title}</Text>
      </Button>
    );
  }
}

const styles = StyleSheet.create({
  btnStyle: {
    height: 50,
    backgroundColor: '#ef5350'
  },
  btnTextStyle: {
    fontSize: 16,
    fontFamily: 'cereal-medium',
    color: 'white'
  }
});
