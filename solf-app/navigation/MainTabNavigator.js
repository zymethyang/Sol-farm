import React from 'react';
import { createStackNavigator, createBottomTabNavigator } from 'react-navigation';

import TabBarIcon from '../components/TabBarIcon';
import InfoScreen from '../screens/InfoScreen';
import ManualScreen from '../screens/ManualScreen';

const InfoStack = createStackNavigator({
  InfoScreen: InfoScreen,
});

InfoStack.navigationOptions = {
  tabBarLabel: 'InfoScreen',
  tabBarIcon: ({ focused }) => (
    <TabBarIcon
      focused={focused}
      name='ios-list'
    />
  ),
  tabBarOptions: {
    showLabel: false,
    showIcon: true,
    tintColor: '#333',
    activeTintColor: '#aaa',
  }
};

const ManualStack = createStackNavigator({
  ManualScreen: ManualScreen,
});

ManualStack.navigationOptions = {
  tabBarLabel: 'ManualScreen',
  tabBarIcon: ({ focused }) => (
    <TabBarIcon
      focused={focused}
      name='ios-build'
    />
  ),
  tabBarOptions: {
    showLabel: false,
    showIcon: true,
    tintColor: '#333',
    activeTintColor: '#aaa',
  }
};

export default createBottomTabNavigator({
  InfoStack,
  ManualStack
});
