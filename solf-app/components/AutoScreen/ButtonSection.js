import React from 'react';
import {
  StyleSheet,
  View,
} from 'react-native';
import HalfBtn from '../HalfBtn';

export default class ButtonSection extends React.Component {
  render() {
    return (
      <View style={styles.container}>
        <View style={styles.wrapBtnLeft}>
          <HalfBtn title="Change tree" onPress={() => this.props.onPress({ type: 'change_tree' })} />
        </View>
        <View style={styles.wrapBtnRight}>
          <HalfBtn title="Stop system" onPress={() => this.props.onPress({ type: 'stop_system' })} />
        </View>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flexDirection: 'row',
    paddingTop: 30
  },
  wrapBtnLeft: {
    flex: 1,
    marginLeft: 20,
    marginRight: 10
  },
  wrapBtnRight: {
    flex: 1,
    marginLeft: 10,
    marginRight: 20
  }
});
