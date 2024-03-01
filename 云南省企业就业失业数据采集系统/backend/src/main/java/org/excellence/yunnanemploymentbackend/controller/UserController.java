package org.excellence.yunnanemploymentbackend.controller;

import io.swagger.annotations.Api;
import io.swagger.annotations.ApiOperation;
import io.swagger.annotations.ApiParam;
import org.excellence.yunnanemploymentbackend.entity.Response;
import org.excellence.yunnanemploymentbackend.entity.User;
import org.excellence.yunnanemploymentbackend.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.bind.annotation.CrossOrigin;
@RestController
@RequestMapping(path = "user")
@Api(tags = "用户控制器")
@CrossOrigin("*")
public class UserController {
    @Autowired
    private UserService userService;

    @ApiOperation(value = "用户登录")
    @PostMapping(path = "login")
    public @ResponseBody Response<User> login(@ApiParam(value = "用户json") @RequestBody User user) {
        try {
            final var result = userService.login(user);
            return result.map(value -> new Response<>(true, "success", value)).orElseGet(() -> new Response<>(true, "failure", null));
        } catch (Exception e) {
            return new Response<>(false, e.getMessage(), null);
        }

    }

    @ApiOperation(value = "用户注册")
    @PostMapping(path = "register")
    public @ResponseBody Response<Boolean> register(@ApiParam(value = "用户json") @RequestBody User user) {
        final var result = userService.register(user);
        if (!result.equals("success")) {
            return new Response<>(true, result, false);
        }
        return new Response<>(true, result, true);
    }
}
