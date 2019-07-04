import React from 'react';
import {
  StyleSheet,
  View
} from 'react-native';
import { Button, Text } from 'native-base';

export default class Btn extends React.Component {
  static navigationOptions = {
    header: null,
  };

  render() {
    let { title } = this.props;
    return (
      <View style={styles.wrapFooterStyle}>
        <Button full style={styles.btnStyle} onPress={() => this.props.onPress()}>
          <Text style={styles.btnTextStyle}>{title}</Text>
        </Button>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  wrapFooterStyle: {
    marginTop: 20,
    width: '100%',
  },
  btnStyle: {
    marginLeft: 20,
    marginRight: 20,
    marginBottom: 20,
    height: 50,
    backgroundColor: '#ef5350'
  },
  btnTextStyle: {
    fontSize: 16,
    fontFamily: 'cereal-medium',
    color: 'white'
  }
});
