import React from 'react';
import {
  StyleSheet,
  View,
  Image
} from 'react-native';

export default class ImageSection extends React.Component {
  render() {
    let { source } = this.props;
    return (
      <View style={styles.container}>
        <Image
          style={styles.imgStyle}
          source={source}
        />
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1, 
    marginLeft: 20, 
    marginRight: 20, 
    paddingTop: 20
  },
  imgStyle: {
    width: null,
    height: 180,
  }
});
