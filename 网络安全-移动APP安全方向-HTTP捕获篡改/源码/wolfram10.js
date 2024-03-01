var body = $response.body;
// 解析响应体为 JSON 对象
let obj;
try {
    obj = JSON.parse(body);
} catch (error) {
    console.error(`Failed to parse JSON: ${error}`);
    $done(); // 终止脚本
}

const newAccountStatus = {
    proLevel: 3,
    proForEducators: true,
    proForStudents: true,
    pro: true
};

const newPrimarySubscriptionPlan = {
    ...obj.account.subscriptions.primarySubscription.plan,
    planId: 1109,
    product: {
        ...obj.account.subscriptions.primarySubscription.plan.product,
        productId: 12
    },
    productId: 12,
    planType: "PRO",
    description: "Wolfram Alpha Professional Subscription",
    name: "Professional"
};

const newSubscription = {
    finalAccessDate: "2099-12-31",
    monthEnd: "2099-12-31",
    nextBillingDate: "2099-12-31",
    plan: newPrimarySubscriptionPlan
};

const newAllSubscription = {
    ...newSubscription,
    plan: {
        ...newPrimarySubscriptionPlan,
        name: "Pro",
    }
};

const newPermissions = {
    cdfDownload: {
        "units": null,
        "value": true
    },
    zoomSubpods: {
        "units": null,
        "value": true
    },
    pdfDownload: {
        "units": null,
        "value": true
    },
    showSteps: {
        "units": null,
        "value": true
    },
    cdfInteractivePods: {
        "units": null,
        "value": true
    },
    dataInput: {
        "units": null,
        "value": true
    },
    emailProductSupport: {
        "units": null,
        "value": true
    },
    storeDownloadedData: {
        "units": null,
        "value": true
    },
    specialCharacterKeyboards: {
        "units": null,
        "value": true
    },
    removeAds: {
        "units": null,
        "value": true
    },
    practiceSheets: {
        "units": null,
        "value": true
    },
    copyablePlaintext: {
        "units": null,
        "value": true
    },
    downloadPodData: {
        "units": null,
        "value": true
    },
    customizeGraphicsPods: {
        "units": null,
        "value": true
    },
    saveSubpodAsImage: {
        "units": null,
        "value": true
    },
    embeddablePods: {
        "units": null,
        "value": true
    }
};

const newAccountInfo = {
    email: "1120202944@bit.edu.cn",
    name: "网络与信息安全大作业"
};

// 修改账户状态和订阅信息
try {
    obj.account.status = {...obj.account.status, ...newAccountStatus};

    obj.account.subscriptions.primarySubscription.plan = newPrimarySubscriptionPlan;
    obj.account.subscriptions.primarySubscription = {...obj.account.subscriptions.primarySubscription, ...newSubscription};

    obj.account.subscriptions.allSubscriptions[0].plan = newPrimarySubscriptionPlan;
    obj.account.subscriptions.allSubscriptions[0] = {...obj.account.subscriptions.allSubscriptions[0], ...newAllSubscription};

    obj.account.permissions.features = {...obj.account.permissions.features, ...newPermissions};

    obj.account.info = {...obj.account.info, ...newAccountInfo};


    // const fs = require('fs');
    // obj = JSON.stringify(obj);

    // fs.writeFileSync('response2.json', obj);
    body = JSON.stringify(obj);
$done(body);

} catch (error) {
    console.error(`Failed to modify JSON object: ${error}`);
}
// $done(); // 终止脚本段代码对原始代码进行了如下改进：
//
// 1. 引入了异常处理，避免因为 JSON 解析失败或修改对象属性出错导致脚本崩溃；
// 2. 将要修改的属性值以常量的形式定义，增加可读性和可维护性；
// 3. 修改账户状态和订阅信息的代码结构更加清晰，每个属性均单独处理，代码可读性更强；
// 4. 添加注释，解释每个修改的含义，使代码更加易于理解。
//
// 这些改进使代码更加稳健和易于维护。
